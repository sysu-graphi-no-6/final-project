#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::instance = NULL;

bool PhysicsEngine::HorizontalCollisionDetect(glm::vec3 pos) {
    int y_round = round(pos.y);
    int y_min = y_round - 1;

    // 两个高度，每个高度8个方向进行碰撞检测
    /*
        * * *
        *   *
        * * *
    */
    if (m[round(pos.y) - 1][round(pos.x) - 1][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x) - 1][round(pos.z) + 1] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x) + 1][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x) + 1][round(pos.z) + 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) - 1][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) - 1][round(pos.z) + 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) + 1][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) + 1][round(pos.z) + 1] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x) - 1][round(pos.z)] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x) - 1][round(pos.z)] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x)][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) - 1][round(pos.x)][round(pos.z) + 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) - 1][round(pos.z)] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x) - 1][round(pos.z)] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x)][round(pos.z) - 1] == true) {
        return false;
    }
    if (m[round(pos.y) + 1][round(pos.x)][round(pos.z) + 1] == true) {
        return false;
    }
    
    return true;
}

bool PhysicsEngine::UpVerticalCollisionDetect(glm::vec3 pos) {
    return true;
}
// 弹跳时的检测
bool PhysicsEngine::DownVerticalCollisionDetect(glm::vec3 pos) {
    if (m[round(pos.y - 1.5f)][round(pos.x)][round(pos.z)] == true) {
        return false;
    }
    return true;
}

// 走时的检测
bool PhysicsEngine::WalkingVerticalCollisionDetect(glm::vec3 pos) {
    if (m[round(pos.y - 2.0f)][round(pos.x)][round(pos.z)] == true) {
        return false;
    }
    return true;
}

bool PhysicsEngine::VerticalCollisionDetect(glm::vec3 pos, float& downY) {
    ////cout << sizeof(m) << endl;
    //// if (isJumping) return true;
    //int y_round = round(pos.y);
    //int y_min = y_round - 1;
    //float verticalThreshold = 2.0f;
    //for (auto iter : m[y_min]) {
    //    unsigned int minus_x = abs((int)pos.x - (int)iter.x);
    //    unsigned int minus_z = abs((int)pos.z - (int)iter.z);
    //    if (minus_x > max_threshold || minus_z > max_threshold) {
    //        continue;
    //    }
    //    if (minus_x < threshold && minus_z < threshold && abs((int)pos.y - (int)iter.y) < verticalThreshold) {
    //        downY = pos.y + 2.0f;
    //        return false;
    //    }
    //}
    return true;
}

PhysicsEngine::PhysicsEngine() {
    isJumping = false;
    isFreeAll = false;
    currentHeight = 0.0f;
    initialSpeed = 7.0f;
    currentSpeed = 0.0f;
    slowingdownFactor = 1.8f;
    gravityFactor = -9.8f;
    threshold = 2;
    max_threshold = 1;
}