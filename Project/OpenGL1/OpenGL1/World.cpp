#include "World.h"
World* World::instance = NULL;


void World::Render(Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    // 画出草地
    glm::vec3 grass_position[460];
    int grass_count = 460;
    int index = 0;
    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            glm::vec3 pos = glm::vec3((float)i, 0.0f, (float)j);
            grass_position[index++] = pos;
            // map可以直接遍历所有行
            engine->m[round(pos.y)].push_back(pos);
        }
    }
    for (int i = -5; i < 5; i++) {
        for (int j = 6; j < 9; j++) {
            glm::vec3 pos = glm::vec3((float)i, 1.0f, (float)j);
            grass_position[index++] = pos;
            // map可以直接遍历所有行
            engine->m[round(pos.y)].push_back(pos);
        }
    }
    SingleRender(shader, grass_count, grass_position, manager->GRASS);

   // 建立围墙
    glm::vec3 brick_position[400];
    int brick_count = 400;
    index = 0;
    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3((float)i, (float)j, -10.0f);
            brick_position[index++] = pos;
            engine->m[round(pos.y)].push_back(pos);
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3(-10.0f, (float)j, (float)i);
            brick_position[index++] = pos;
            engine->m[round(pos.y)].push_back(pos);
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3((float)i, (float)j, 9.0f);
            brick_position[index++] = pos;
            engine->m[round(pos.y)].push_back(pos);
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3(9.0f, (float)j, (float)i);
            brick_position[index++] = pos;
            engine->m[round(pos.y)].push_back(pos);
        }
    }
    SingleRender(shader, brick_count, brick_position, manager->BRICK);

}

void World::SingleRender(Shader& shader, int count, glm::vec3* position, ResourceManager::BlockType block) {
    ResourceManager* manager = ResourceManager::getInstance();
    glBindBuffer(GL_ARRAY_BUFFER, manager->getInstanceVBO());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * count, &position[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    manager->RenderScene(shader, block, count);
}
