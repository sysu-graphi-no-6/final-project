#pragma once
#include "ResourceManager.h"
#include "Shader.h"
#include "PhysicsEngine.h"
class World
{
public:
    World(){}
    ~World(){}

    static World* getInstance() {
        if (instance == NULL) {
            instance = new World();
        }
        return instance;
    }

    void Render(Shader& shader);
    // µ•∂¿Ω¯––‰÷»æ
    void SingleRender(Shader& shader, int count, glm::vec3* position, ResourceManager::BlockType block);

private:
    static World* instance;
};