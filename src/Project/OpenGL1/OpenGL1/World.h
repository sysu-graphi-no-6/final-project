#pragma once
#include "ResourceManager.h"
#include "Shader.h"
#include "PhysicsEngine.h"

//该类管理整个世界的方块
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
    // 单独进行渲染
    void SingleRender(Shader& shader, int count, glm::vec3* position, ResourceManager::BlockType block);
	//渲染模型
	void Rendermodel(Shader& shader, int count, glm::vec3* position, ResourceManager::ModelType model);
    // 画出树
    void DrawTree(ResourceManager::BlockType tree_type, ResourceManager::BlockType leave_type, glm::vec3 pos, Shader& shader);
private:
    static World* instance;
};