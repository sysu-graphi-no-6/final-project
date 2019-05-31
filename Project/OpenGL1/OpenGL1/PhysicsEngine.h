#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
using namespace std;


class PhysicsEngine {
public:
    PhysicsEngine();

    static PhysicsEngine* getInstance() {
        if (instance == NULL) instance = new PhysicsEngine();
        return instance;
    }

    void InitJumping() {
        isJumping = true;
        currentSpeed = initialSpeed;
        currentHeight = 0;
    }

    bool CollisionDetect(glm::vec3 pos);
    //------碰撞检测参数-------
    float threshold = 1.0f;

    //----------弹跳相关系数 ----------------------------
    float currentHeight;
    float initialSpeed;
    float currentSpeed;
    float slowingdownFactor;
    float gravityFactor;
    bool isJumping;
    // 利用map 只对高度进行遍历
    map<int, vector<glm::vec3> > m;

private:

    static PhysicsEngine* instance;
};
