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
    JUMP,
    FLYSKY
};

/*定义相机的相关类*/
class Camera {
public:

    // 属性值
    // 相机的位置
    glm::vec3 Position = glm::vec3(0.0f, 1.0f, 0.0f);
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
    void ProcessKeyboard(Direction direction, float deltaTime)
    {
        PhysicsEngine* engine = PhysicsEngine::getInstance();

        // 通过移动速度以及持续时间获得速度engine-
        float cameraSpeed = MovementSpeed * deltaTime;
        if (direction == FLYSKY) {
            if(!flysky)cout << "飞天模式打开！" << endl;
            else cout << "飞天模式关闭！" << endl;
            flysky = !flysky;
            return;
        }
        // 飞天模式是否开启
        if (flysky) {
            glm::vec3 afterMove;
            // 进行四种方向的判断
            if (direction == FORWARD)
                afterMove = moveForward(Front * cameraSpeed);
            if (direction == BACKWARD)
                afterMove = moveBack(Front * cameraSpeed);
            if (direction == LEFT)
                afterMove = moveLeft(glm::normalize(glm::cross(Front, Up)) * cameraSpeed);
            if (direction == RIGHT)
                afterMove = moveRight(glm::normalize(glm::cross(Front, Up)) * cameraSpeed);
            if (direction == JUMP)
                afterMove = glm::vec3(Position.x, Position.y + 0.01f, Position.z);
            Position = afterMove;
        }
        else {
            float forwardVector = 0.0f, rightVector = 0.0f;
            // 进行四种方向的判断
            if (direction == FORWARD)
                forwardVector += 2.0f;
            if (direction == BACKWARD)
                forwardVector -= 2.0f;
            if (direction == LEFT)
                rightVector -= 2.0f;
            if (direction == RIGHT)
                rightVector += 2.0f;
            if (forwardVector != 0.0f || rightVector != 0.0f) {
                //行走不改变y轴坐标
                glm::mat4 vm = getViewMatrix();
                glm::vec3 forward = glm::vec3(vm[0][2], 0.0f, vm[2][2]);
                glm::vec3 strafe = glm::vec3(vm[0][0], 0.0f, vm[2][0]);

                Position += (-forwardVector * forward + rightVector * strafe) * cameraSpeed;
                targetPos = Position + (-forwardVector * forward + rightVector * strafe) * 1.5f;

            }
            if (direction == JUMP && !engine->isJumping) {
                engine->InitJumping();
            }
        }
        
        
    }
    // 进行当前位置的计算(弹跳期间)
    void UpdatePositionEachSecond(float deltaTime) {
       /* cout << Position.y << endl;*/
        PhysicsEngine* engine = PhysicsEngine::getInstance();
        if (engine->isJumping) {
            // 利用重力加速度参数 (v + v0)/ 2 * t
            if ((engine->currentSpeed > 0.0f && engine->currentHeight == 0.0f) || engine->currentHeight >= 0.0f) {
                float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime;
                engine->currentHeight += (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
                if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;
                cout << "Height:" << (engine->currentHeight)<<"Speed"<<(engine->currentSpeed) << endl;
                engine->currentSpeed = nowSpeed;
                // 判断是否是上升阶段，如果是则加上相应高度，否则减去
                if (engine->currentSpeed >= 0) {
                    Position = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
                }
                else {
                    Position = glm::vec3(Position.x, Position.y - engine->currentHeight, Position.z);
                }
            }
            else if (engine->currentSpeed <= -(engine->initialSpeed)) {
                engine->isJumping = false;
                Position = glm::vec3(Position.x, floor(Position.y - engine->currentHeight), Position.z);
             }
        }
    }
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


private:
    /**
    * @brief 前向移动
    * @param distance 移动距离
    **/
    glm::vec3 moveForward(glm::vec3 distance) {
        return Position + distance;
    }

    /**
    * @brief 前后移动
    * @param distance 移动距离
    **/
    glm::vec3 moveBack(glm::vec3 distance) {
        return Position - distance;
    }

    /**
    * @brief 前右移动
    * @param distance 移动距离
    **/
    glm::vec3 moveRight(glm::vec3 distance) {
        return Position + distance;
    }

    /**
    * @brief 前左移动
    * @param distance 移动距离
    **/
    glm::vec3 moveLeft(glm::vec3 distance) {
        return Position - distance;
    }

    static Camera* instance;
};


#endif