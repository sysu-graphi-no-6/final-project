#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PhysicsEngine.h"

#define roomSizeX 200.f
#define roomSizeY 60.f
#define roomSizeZ 200.f
// 通过集中可能的相机移动进行相机视图的变化
enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT,
    JUMP,
    FLYSKY
};

/*定义相机的相关类*/
class Camera {
public:

    // 属性值
    // 相机的位置
    glm::vec3 Position = glm::vec3(0.0f, 2.0f, 0.0f);
    // 相机的前向量
    glm::vec3 Front = glm::vec3(1.0f, 0.0f, 0.0f);
    // 相机的上向量
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    // 相机的右向量
    glm::vec3 Right;
    // 观察坐标
    glm::vec3 WorldUp = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 targetPos = glm::vec3(5.0f, 0.0f, 0.0f);

    // 判断是否飞天模式
    bool flysky = false;
    // 欧拉角
    float Yaw = -90.0f;
    float Pitch = 0.0f;

    // 用于判断最后鼠标位置
    float lastX;
    float lastY;
    float fov = 45.0f;
    // 是否第一次移动鼠标
    bool firstMouse = true;

    // 相机的选项
    float MovementSpeed = 3.0f;
    // 鼠标敏感度
    float MouseSensitivity = 0.1f;
    // 缩放比例
    float Zoom = 60.0f;

    static Camera* getInstance() {
        if (instance == NULL) {
            instance = new Camera();
        }
        return instance;
    }

    /**
    * @brief 通过获得的方向进行动作的处理
    * @param direction 输入的方向
    * @param deltaTIme 输入的时间
    */
    void ProcessKeyboard(Direction direction, float deltaTime);

    // 进行当前位置的计算(弹跳期间)
    void UpdatePositionEachSecond(float deltaTime);
    /**
    * @brief 鼠标移动从而移动视图
    * @param xoffset x方向偏移量
    * @param yoffset y方向偏移量
    */
    void lookAround(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }
    
    /**
    * @brief 设置鼠标最后位置
    * @param x x坐标
    * @param y y坐标
    */
    void setLastXY(float x, float y) {
        lastX = x;
        lastY = y;
    }

    void setPosition(glm::vec3 newPos) {
        this->Position = newPos;
    }

    void setFront(glm::vec3 front) {
        this->Front = front;
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // 保留若干位小数
    static float getFloat(float input, int n) {

        return (float)(round(input * pow(10, n))) / (float)(pow(10, n));
    }


private:
    /**
    * @brief 前向移动
    * @param distance 移动距离
    **/
    glm::vec3 moveForward(glm::vec3 distance, glm::vec3 pos) {
        return pos + distance;
    }

    /**
    * @brief 前后移动
    * @param distance 移动距离
    **/
    glm::vec3 moveBack(glm::vec3 distance, glm::vec3 pos) {
        return pos - distance;
    }

    /**
    * @brief 前右移动
    * @param distance 移动距离
    **/
    glm::vec3 moveRight(glm::vec3 distance, glm::vec3 pos) {
        return pos + distance;
    }

    /**
    * @brief 前左移动
    * @param distance 移动距离
    **/
    glm::vec3 moveLeft(glm::vec3 distance, glm::vec3 pos) {
        return pos - distance;
    }

    static Camera* instance;
};


#endif