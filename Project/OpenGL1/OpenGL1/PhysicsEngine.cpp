#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::instance = NULL;

bool PhysicsEngine::CollisionDetect(glm::vec3 pos) {
    //cout << sizeof(m) << endl;
    if (isJumping) return true;
    int y_round = round(pos.y);
    int count = 0;
    for (auto iter : m[y_round]) {
        count++;
        if (abs(pos.x - iter.x) < 1.5f && abs(pos.z - iter.z) < 1.5f) {
           // cout << count << endl;
            return false;
        }
    }
    return true;
}

PhysicsEngine::PhysicsEngine() {
    isJumping = false;
    currentHeight = 0.0f;
    initialSpeed = 5.0f;
    currentSpeed = 0.0f;
    slowingdownFactor = 2.0f;
    gravityFactor = -9.8f;
}