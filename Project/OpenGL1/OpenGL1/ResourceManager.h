#pragma once
#include <iostream>
#include "Initial.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <map>
using namespace std;
#define MAX_BLOCK 6400
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
    }

    // ----------纹理类-----------------------
    // 方块种类
    enum BlockType {
        GRASS, BRICK
    };
    // 加载纹理
    unsigned int loadTexture(GLchar* path, unsigned int format);
    void setAllTexture() {
        textureID["grass_side"] = loadTexture("resource/blocks/grass_path_side.png", GL_RGBA);
        textureID["grass_top"] = loadTexture("resource/blocks/grass_path_top.png", GL_RGB);
        textureID["dirt"] = loadTexture("resource/blocks/dirt.png", GL_RGB);
        textureID["brick"] = loadTexture("resource/blocks/brick.png", GL_RGB);
    }
    //----------------渲染----------------------------
    // 选择渲染的方向
    enum RenderDirection
    {
        LEFT, RIGHT, TOP, BOTTOM, BACK, FRONT
    };
    // 在while循环中的渲染
    void RenderScene(Shader &shader, BlockType blockType, unsigned int drawCount = 1)
    {
        // 物体1
        glm::mat4 model = glm::mat4(1.0f);
       // model = glm::translate(model, pos);
        shader.setMat4("model", model);
        // RenderCube(grass_top);
        switch (blockType)
        {
        case GRASS:
            RenderFace(textureID["grass_top"], TOP, drawCount);
            RenderFace(textureID["dirt"], BOTTOM, drawCount);
            RenderFace(textureID["grass_side"], LEFT, drawCount);
            RenderFace(textureID["grass_side"], RIGHT, drawCount);
            RenderFace(textureID["grass_side"], FRONT, drawCount);
            RenderFace(textureID["grass_side"], BACK, drawCount);
            break;
        case BRICK:
            RenderCube(textureID["brick"], drawCount);
            break;
        default:
            RenderFace(textureID["grass_top"], TOP, drawCount);
            RenderFace(textureID["dirt"], BOTTOM, drawCount);
            RenderFace(textureID["grass_side"], LEFT, drawCount);
            RenderFace(textureID["grass_side"], RIGHT, drawCount);
            RenderFace(textureID["grass_side"], FRONT, drawCount);
            RenderFace(textureID["grass_side"], BACK, drawCount);
            break;
        }

    }
    // 渲染整个方块
    void RenderCube(unsigned int texture, unsigned int drawCount = 1);
    // 进行方块的初始化
    void InitCube() {
        // 最大支持40 * 40
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_BLOCK, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Single_InitCube(cubeVAO_top, cubeVBO_top, TOP);
        Single_InitCube(cubeVAO_bottom, cubeVBO_bottom, BOTTOM);
        Single_InitCube(cubeVAO_front, cubeVBO_front, FRONT);
        Single_InitCube(cubeVAO_back, cubeVBO_back, BACK);
        Single_InitCube(cubeVAO_left, cubeVBO_left, LEFT);
        Single_InitCube(cubeVAO_right, cubeVBO_right, RIGHT);
    }
    void Single_InitCube(unsigned int& VAO, unsigned int &VBO, RenderDirection dir){
        // top
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // 根据方向渲染
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (dir == TOP) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);
        }
        else if (dir == BOTTOM) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);
        }
        else if (dir == FRONT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(front), front, GL_STATIC_DRAW);
        }
        else if (dir == BACK) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);
        }
        else if (dir == LEFT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(left), left, GL_STATIC_DRAW);
        }
        else if (dir == RIGHT) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(right), right, GL_STATIC_DRAW);
        }
        // Link vertex attributes
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(3, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
       // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //  渲染单独一面
    void RenderFace(unsigned int texture, RenderDirection dir, unsigned int drawCount);

    //----------天空盒-------------------------
    // 加载天空盒的纹理
    unsigned int loadCubemap(vector<std::string> faces);
    // 进行天空盒的渲染准备
    void InitSky();
    // 进行天空盒的渲染
    void RenderSky(glm::mat4 projection);
    unsigned int getInstanceVBO() {
        return instanceVBO;
    }

private:
    // 材质和对应的id
    map<string, unsigned int> textureID;
    unsigned int instanceVBO;
    static ResourceManager* instance;
    // --------------渲染部分-----------------------
    GLfloat front[48] = {
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom-left
    };
    GLfloat back[48] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// top-left      
    };
    GLfloat left[48] = {
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right

    };
    GLfloat right[48] = {
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left     

    };
    GLfloat bottom[48] = {
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right

    };

    GLfloat top[48] = {
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f // bottom-left        
    };

    // ------------- 方块部分-------------------------
    unsigned int cubeVBO_top, cubeVBO_bottom, cubeVBO_left, cubeVBO_right, cubeVBO_front, cubeVBO_back;
    unsigned int cubeVAO_top, cubeVAO_bottom, cubeVAO_left, cubeVAO_right, cubeVAO_front, cubeVAO_back;
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