#include "ShaderManager.h"
ShaderManager* ShaderManager::instance = NULL;

void ShaderManager::InitialShader() {
    blockShader.setText("shadow.vs", "shadow.frag");
    simpleDepthShader.setText("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");

    // 设置平行光的参数
    blockShader.setVec3("dir", glm::vec3(-10.0f, 0.0f, 0.0f));

    // 设置方块着色器参数 
    blockShader.setInt("diffuseTexture", 0);
    blockShader.setInt("shadowMap", 1);
}