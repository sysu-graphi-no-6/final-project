#include "World.h"
World* World::instance = NULL;

void World::Render(Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    // 渲染出草地
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            // 物体反光特性设置
            shader.setFloat("ambientStrength", 0.3f);
            shader.setFloat("shininess", 64.0f);
            shader.setFloat("diffuseFactor", 1.0f);
            shader.setFloat("specularStrength", 1.0f);
            shader.setVec3("lightColor", glm::vec3(1.0f));
            manager->RenderScene(shader, glm::vec3((float)i, 0.0f, (float)j), manager->GRASS);
        }
    }
   // manager->RenderScene(shader, glm::vec3(1.0f, 3.0f, 2.0f), manager->GRASS);
}