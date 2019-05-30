#pragma once
#include <iostream>
#include "Initial.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

/*进行资源管理，比如画出立方体等*/
class ResourceManager {
public:
   
    static ResourceManager* getInstance() {
        if (instance == NULL) {
            instance = new ResourceManager();
        }
        return instance;
    }
    ResourceManager() {
        cubeVAO = 0;
        cubeVBO = 0;
    }

    // ----------纹理类-----------------------
    // 方块种类
    enum BlockType {
        GRASS
    };
    // 加载纹理
    unsigned int loadTexture(GLchar* path);
    void setAllTexture() {
        textureID["grass_side"] = loadTexture("resource/blocks/grass_side.png");
        textureID["grass_top"] = loadTexture("resource/blocks/cobblestone_mossy.png");
    }
    //----------------渲染----------------------------
    // 选择渲染的方向
    enum RenderDirection
    {
        LEFT, RIGHT, TOP, BOTTOM, BACK, FRONT
    };
    // 在while循环中的渲染
    void RenderScene(Shader &shader, glm::vec3 pos, BlockType blockType)
    {
        // 物体1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        shader.setMat4("model", model);
        // RenderCube(grass_top);
        switch (blockType)
        {
        case GRASS:
            RenderFace(textureID["grass_top"], TOP);
            RenderFace(textureID["grass_side"], BOTTOM);
            RenderFace(textureID["grass_side"], LEFT);
            RenderFace(textureID["grass_side"], RIGHT);
            RenderFace(textureID["grass_side"], FRONT);
            RenderFace(textureID["grass_side"], BACK);
            break;
        default:
            RenderFace(textureID["grass_top"], TOP);
            RenderFace(textureID["grass_side"], BOTTOM);
            RenderFace(textureID["grass_side"], LEFT);
            RenderFace(textureID["grass_side"], RIGHT);
            RenderFace(textureID["grass_side"], FRONT);
            RenderFace(textureID["grass_side"], BACK);
            break;
        }

    }
    // 渲染整个方块
    void RenderCube(unsigned int texture);

    //  渲染单独一面
    void RenderFace(unsigned int texture, RenderDirection dir);

    //----------天空盒-------------------------
    // 加载天空盒的纹理
    unsigned int loadCubemap(vector<std::string> faces);
    // 进行天空盒的渲染准备
    void InitSky();
    // 进行天空盒的渲染
    void RenderSky(glm::mat4 projection);


private:
    // 材质和对应的id
    map<string, unsigned int> textureID;

    static ResourceManager* instance;
    // --------------渲染部分-----------------------
    GLfloat front[48] = {
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
    };
    GLfloat back[48] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left      
    };
    GLfloat left[48] = {
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right

    };
    GLfloat right[48] = {
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     

    };
    GLfloat bottom[48] = {
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right

    };

    GLfloat top[48] = {
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
    };

    // ------------- 方块部分-------------------------
    unsigned int cubeVAO, cubeVBO;

    //----------------天空盒部分-----------------------------
    unsigned int skyboxVAO, skyboxVBO;
    float skyboxVertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    vector<string> skybox_faces
    {
        "resource/skybox/nevada_lf.jpg",
        "resource/skybox/nevada_rt.jpg",
        "resource/skybox/nevada_up.jpg",
        "resource/skybox/nevada_dn.jpg",
        "resource/skybox/nevada_ft.jpg",
        "resource/skybox/nevada_bk.jpg"
    };
    unsigned int cubemapTexture;
    Shader skyboxShader;
};