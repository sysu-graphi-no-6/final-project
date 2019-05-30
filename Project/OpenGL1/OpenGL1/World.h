#pragma once
#include "ResourceManager.h"
#include "Shader.h"
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

private:
    static World* instance;
};