#pragma once
#include <iostream>
#include "Initial.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <map>
#include "Model.h"
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
    ~ResourceManager() {
        for (auto iter : multipleTextureID) {
            pair<int, unsigned int*> p = iter.second;
            delete(p.second);
        }
    }
    // ----------纹理类-----------------------
    // 方块种类
    enum BlockType {
        GRASS, BRICK, DIRT, WATER, Tree_birch, Tree_oak, Tree_jungle, Leave_oak, Leave_birch, Leave_jungle,
        MUSHROOM, FLOWER,VINE,STONE,
    };
	enum ModelType {
		rose, spider
	};
    // 加载纹理
    unsigned int loadTexture(const GLchar* path);
    // 加载多张图片，比如水
    unsigned int* loadTextures(const GLchar* path, int count);
	//加载模型
	Model& LoadModel(const GLchar* file, std::string name);
	Model& GetModel(std::string name);
	Model loadModelFromFile(const GLchar* file);
    void setAllTexture() {
        textureID["grass_side"] = loadTexture("resource/blocks/grass_path_side.png");
        textureID["grass_top"] = loadTexture("resource/blocks/grass_path_top.png");
        textureID["dirt"] = loadTexture("resource/blocks/dirt.png");
        textureID["brick"] = loadTexture("resource/blocks/brick.png");
        multipleTextureID["water"].first = 32;
        multipleTextureID["water"].second = loadTextures("resource/blocks2/water_still.png", 32);
        // 树干
        textureID["tree_oak_top"] = loadTexture("resource/blocks/log_oak_top.png");
        textureID["tree_oak_side"] = loadTexture("resource/blocks/log_oak.png");
        textureID["tree_birch_top"] = loadTexture("resource/blocks/log_birch_top.png");
        textureID["tree_birch_side"] = loadTexture("resource/blocks/log_birch.png");
        textureID["tree_jungle_top"] = loadTexture("resource/blocks/log_spruce_top.png");
        textureID["tree_jungle_side"] = loadTexture("resource/blocks/log_spruce.png");
        // 树叶 添加颜色50aa55
        textureID["leave_birch"] = loadTexture("resource/blocks/leaves_birch.png");
        textureID["leave_jungle"] = loadTexture("resource/blocks/leaves_jungle.png");
        textureID["leave_oak"] = loadTexture("resource/blocks/leaves_oak.png");
        // 蘑菇
        textureID["mushroom"] = loadTexture("resource/blocks/mushroom_red.png");
		textureID["flower"] = loadTexture("resource/blocks/flower_oxeye_daisy.png");
		textureID["vine"] = loadTexture("resource/blocks/end_stone.png");
		textureID["stone"] = loadTexture("resource/blocks/end_stone.png");
		Models["rose"] = LoadModel("resource/rose/Models and Textures/rose.obj","rose");
    }
    //----------------渲染----------------------------
    // 选择渲染的方向
    enum RenderDirection
    {
        LEFT, RIGHT, TOP, BOTTOM, BACK, FRONT, CROSS
    };
    // 在while循环中方块的渲染(每个面可以不同材质)
    void RenderScene(Shader &shader, BlockType blockType, unsigned int drawCount = 1);
	void RenderModelScene(Shader &shader, ModelType modelType, unsigned int drawCount =1);

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
        Single_InitCube(crossVAO, crossVBO, CROSS);
    }

    // 进行单个方块的绑定
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
        else if (dir == CROSS) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(cross), cross, GL_STATIC_DRAW);
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


    //void LoadTextureSplit(const GLchar* file, int count);
private:
    // 材质和对应的id
    map<string, unsigned int> textureID;
	//模型
	map<string, Model> 		Models;
    // 可以作为动画的材质
    map<string, pair<int, unsigned int*> > multipleTextureID;
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

    GLfloat cross[96] = {
        // Back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        // Front
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
    };

    // ------------- 方块部分-------------------------
    unsigned int cubeVBO_top, cubeVBO_bottom, cubeVBO_left, cubeVBO_right, cubeVBO_front, cubeVBO_back, crossVAO;
    unsigned int cubeVAO_top, cubeVAO_bottom, cubeVAO_left, cubeVAO_right, cubeVAO_front, cubeVAO_back, crossVBO;
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