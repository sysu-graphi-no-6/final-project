#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
using namespace std;


class PhysicsEngine {
public:
    PhysicsEngine() { isJumping = false;}

    static PhysicsEngine* getInstance() {
        if (instance == NULL) instance = new PhysicsEngine();
        return instance;
    }

    void InitJumping() {
        isJumping = true;
        currentSpeed = initialSpeed;
        currentHeight = 0;
    }

    //----------弹跳相关系数 ----------------------------
    float currentHeight = 0.0f;
    float initialSpeed = 2.5f;
    float currentSpeed = 0.0f;
    float slowingdownFactor = 0.02f;
    float gravityFactor = -9.8f;
    bool isJumping;


private:

    static PhysicsEngine* instance;
};
