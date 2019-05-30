#include "World.h"
World* World::instance = NULL;

void World::Render(Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    // äÖÈ¾³ö²İµØ
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            manager->RenderScene(shader, glm::vec3((float)i, 0.0f, (float)j), manager->GRASS);
        }
    }
   // manager->RenderScene(shader, glm::vec3(1.0f, 3.0f, 2.0f), manager->GRASS);
}