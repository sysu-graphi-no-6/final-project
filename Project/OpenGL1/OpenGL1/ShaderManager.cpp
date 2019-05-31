#include "ShaderManager.h"
ShaderManager* ShaderManager::instance = NULL;

void ShaderManager::InitialShader() {
    blockShader.setText("shadow.vs", "shadow.frag");
    simpleDepthShader.setText("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
    directionalLight.setText("multipleLight.vs", "multipleLight.frag");

    // 设置平行光的参数
    directionalLight.use();
    directionalLight.setInt("material.diffuse", 0);
    directionalLight.setInt("material.specular", 1);

    // 设置方块着色器参数 
    blockShader.setInt("diffuseTexture", 0);
    blockShader.setInt("shadowMap", 1);
}