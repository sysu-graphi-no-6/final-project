#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::instance = NULL;

bool PhysicsEngine::HorizontalCollisionDetect(glm::vec3 pos) {
    int y_round = round(pos.y);
    int y_min = y_round - 1;

    //cout << "pos:" << pos.x << "z: " << pos.z << endl;
    for (auto iter : m[y_round]) {
        //太近的排除 减少运算
        
        unsigned int minus_x = abs((int)pos.x - (int)iter.x);
        unsigned int minus_z = abs((int)pos.z - (int)iter.z);
        if (minus_x > max_threshold || minus_z > max_threshold) {
            continue;
        }
        if (minus_x < threshold && minus_z < threshold){
            //cout << "iter:" << iter.x << "z: " << iter.z << endl;
            glm::vec3 a = pos, b = iter;
            return false;
        }
    }
    for (auto iter : m[y_min]) {
        //太近的排除 减少运算
        unsigned int minus_x = abs((int)pos.x - (int)iter.x);
        unsigned int minus_z = abs((int)pos.z - (int)iter.z);
        if (minus_x > max_threshold || minus_z > max_threshold) {
            continue;
        }
        if (minus_x < threshold && minus_z < threshold) {
            return false;
        }
    }
    return true;
}

bool PhysicsEngine::VerticalCollisionDetect(glm::vec3 pos, float& downY) {
    //cout << sizeof(m) << endl;
    // if (isJumping) return true;
    int y_round = round(pos.y);
    int y_min = y_round - 1;
    float verticalThreshold = 2.0f;
    for (auto iter : m[y_min]) {
        unsigned int minus_x = abs((int)pos.x - (int)iter.x);
        unsigned int minus_z = abs((int)pos.z - (int)iter.z);
        if (minus_x > max_threshold || minus_z > max_threshold) {
            continue;
        }
        if (minus_x < threshold && minus_z < threshold && abs((int)pos.y - (int)iter.y) < verticalThreshold) {
            downY = pos.y + 2.0f;
            return false;
        }
    }
    return true;
}

PhysicsEngine::PhysicsEngine() {
    isJumping = false;
    currentHeight = 0.0f;
    initialSpeed = 7.0f;
    currentSpeed = 0.0f;
    slowingdownFactor = 1.5f;
    gravityFactor = -9.8f;
    threshold = 2;
    max_threshold = 1;
}