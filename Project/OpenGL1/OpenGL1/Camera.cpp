#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
Camera* Camera::instance = NULL;

// 进行当前位置的计算(弹跳期间) 因为Camera include PhysicsEngine类，所以不能互相include
void Camera::UpdatePositionEachSecond(float deltaTime) {
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    //cout << "Height: " << (Position.y) << "Speed: " << (engine->currentSpeed) << endl;
    
    if (engine->isJumping) {
        // 利用重力加速度参数 (v + v0)/ 2 * t
        if (engine->currentSpeed > -(engine->initialSpeed)) {
            float nowSpeed = engine->currentSpeed + engine->gravityFactor * deltaTime * engine->slowingdownFactor;
            // 当前走过的路程
            engine->currentHeight = (nowSpeed + engine->currentSpeed) / 2.0f * deltaTime;
            // if (engine->currentHeight < 0.0f && engine->currentHeight > -0.1f) engine->currentHeight = 0.0f;
            // cout << "Height:" << (engine->currentHeight) << "Speed" << (engine->currentSpeed) << endl;
            engine->currentSpeed = nowSpeed;
            glm::vec3 afterMove = glm::vec3(Position.x, Position.y + engine->currentHeight, Position.z);
            // 解决下方碰撞问题
            if (engine->currentSpeed < 0) {
                float downY;
                if (engine->VerticalCollisionDetect(afterMove, downY)) {
                    Position = afterMove;
                }
                else {
                    Position = glm::vec3(Position.x, downY, Position.z);
                }
            }
            else {
                Position = afterMove;
            }
            
        }
        else {
            engine->isJumping = false;
            engine->currentSpeed = 0.0f;
            Position = glm::vec3(Position.x, round(Position.y), Position.z);
            /* cout << Position.y << endl;*/
            
        }
    }
}

void Camera::ProcessKeyboard(Direction direction, float deltaTime)
{
    PhysicsEngine* engine = PhysicsEngine::getInstance();

    // 通过移动速度以及持续时间获得速度engine-
    float cameraSpeed = MovementSpeed * deltaTime;
    if (direction == FLYSKY) {
        if (!flysky)cout << "飞天模式打开！" << endl;
        else cout << "飞天模式关闭！" << endl;
        flysky = !flysky;
        return;
    }
    glm::vec3 afterMove(1.0f);
    float forwardVector = 0.0f, rightVector = 0.0f;
    // 不在最后一刻进行处理以避免多次碰撞检测出现问题
    if (engine->isJumping && engine->currentSpeed <= -(engine->initialSpeed)) return;
    // 进行四种方向的判断
    float speed = 1.0f;
    if (direction == FORWARD)
        forwardVector += speed;
    if (direction == BACKWARD)
        forwardVector -= speed;
    if (direction == LEFT)
        rightVector -= speed;
    if (direction == RIGHT)
        rightVector += speed;
    if (direction == FORWARD_LEFT) {
        forwardVector += speed;
        rightVector -= speed;
    }
    if (direction == FORWARD_RIGHT) {
        forwardVector += speed;
        rightVector += speed;
    }
    if (direction == BACKWARD_LEFT) {
        forwardVector -= speed;
        rightVector -= speed;
    }
    if (direction == BACKWARD_RIGHT) {
        forwardVector -= speed;
        rightVector += speed;
    }
    if (forwardVector != 0.0f || rightVector != 0.0f) {
        //行走不改变y轴坐标
        glm::mat4 vm = getViewMatrix();
        glm::vec3 forward = glm::vec3(vm[0][2], 0.0f, vm[2][2]);
        glm::vec3 strafe = glm::vec3(vm[0][0], 0.0f, vm[2][0]);

        afterMove = Position + (-forwardVector * forward + rightVector * strafe) * cameraSpeed;
        // 保留小数，减少计算量
        int remain = 3;
        afterMove = glm::vec3(getFloat(afterMove.x, remain), getFloat(afterMove.y, remain), getFloat(afterMove.z, remain));
        //cout << afterMove.x << " " << afterMove.y << " " << afterMove.z << endl;
       //afterMove = glm::vec3((int)afterMove.x, (int)afterMove.y, (int)afterMove.z);
        if (PhysicsEngine::getInstance()->HorizontalCollisionDetect(afterMove)) {
            // 如果下面没有方块，掉下去
            bool blockDown = false;
            for (auto iter : engine->m[(int)afterMove.y - 2]) {
                //太近的排除 减少运算
                unsigned int minus_x = abs((int)afterMove.x - (int)iter.x);
                unsigned int minus_z = abs((int)afterMove.z - (int)iter.z);
                if (minus_x > engine->max_threshold || minus_z > engine->max_threshold) {
                    continue;
                }
                if (minus_x < engine->threshold && minus_z < engine->threshold) {
                    blockDown = true;
                    break;
                }
            }
            if (!blockDown && !engine->isJumping) {
                afterMove = glm::vec3(afterMove.x, afterMove.y - 1, afterMove.z);
                engine->isJumping = false;
            }
            Position = afterMove;
        }
    }
    if (!flysky && direction == JUMP && !engine->isJumping) {
        engine->InitJumping();
    }
    if(flysky && direction == JUMP)
        Position = glm::vec3(Position.x, Position.y + 0.01f, Position.z);
}