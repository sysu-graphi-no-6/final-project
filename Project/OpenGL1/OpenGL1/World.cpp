#include "World.h"
World* World::instance = NULL;

void World::Render(Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    glm::vec3 position[5] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 0.0f, 0.0f),
    glm::vec3(3.0f, 0.0f, 0.0f),
    glm::vec3(4.0f, 0.0f, 0.0f),

    };

    glBindBuffer(GL_ARRAY_BUFFER, manager->getInstanceVBO());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * 5, &position[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
   manager->RenderScene(shader, manager->GRASS, 5);
}