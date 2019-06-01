#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
ResourceManager* ResourceManager::instance = NULL;

void ResourceManager::RenderFace(unsigned int texture, RenderDirection dir, unsigned int drawCount) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (dir == TOP) {
        glBindVertexArray(cubeVAO_top);
    }
    else if (dir == BOTTOM) {
        glBindVertexArray(cubeVAO_bottom);
    }
    else if (dir == FRONT) {
        glBindVertexArray(cubeVAO_front);
    }
    else if (dir == BACK) {
        glBindVertexArray(cubeVAO_back);
    }
    else if (dir == LEFT) {
        glBindVertexArray(cubeVAO_left);
    }
    else if (dir == RIGHT) {
        glBindVertexArray(cubeVAO_right);
    }
    if (dir != CROSS) {
        // Render Cube
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, drawCount);
        glBindVertexArray(0);
    }
    else {
        glBindVertexArray(crossVAO);
        // Render Cube
        glDrawArraysInstanced(GL_TRIANGLES, 0, 12, drawCount);
        glBindVertexArray(0);
    }
}

void ResourceManager::RenderCube(unsigned int texture, unsigned int drawCount) {
    RenderFace(texture, TOP, drawCount);
    RenderFace(texture, BOTTOM, drawCount);
    RenderFace(texture, LEFT, drawCount);
    RenderFace(texture, RIGHT, drawCount);
    RenderFace(texture, FRONT, drawCount);
    RenderFace(texture, BACK, drawCount);
}

unsigned int ResourceManager::loadTexture(GLchar* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

unsigned int* ResourceManager::loadTextures(GLchar* path, int count) {
    unsigned int* textures = new unsigned int[count];
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        // 进行图片的切分
        int singleHeight = height / count;

        for (int i = 0; i < count; i++) {
            // Create Texture
            glGenTextures(1, &textures[i]);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, singleHeight, 0, format, GL_UNSIGNED_BYTE, data + (nrChannels * width * singleHeight) * i);
            //glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        stbi_image_free(data);
        return textures;
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return NULL;
    }
    return NULL;
}

/* 下列函数接受留个纹理路径的vector*/
unsigned int ResourceManager::loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void ResourceManager::InitSky() {
    // 天空盒
    skyboxShader.setText("skybox.vs", "skybox.frag");
    cubemapTexture = loadCubemap(skybox_faces);
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 设置天空盒
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
}

void ResourceManager::RenderSky(glm::mat4 projection) {
    // 画出天空盒
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.use();
    glm::mat4 view = glm::mat4(glm::mat3(Camera::getInstance()->getViewMatrix())); // remove translation from the view matrix
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

// 在while循环中方块的渲染
void ResourceManager::RenderScene(Shader &shader, BlockType blockType, unsigned int drawCount)
{
    // 记录水的状态
    static unsigned int water_state = 0;
    // 记录帧数，减缓速度
    static unsigned int frame = 0;
    // 物体1
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, pos);
    shader.setMat4("model", model);
    // RenderCube(grass_top);
    // 这里添加方块类型需要添加枚举型
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
    case DIRT:
        RenderCube(textureID["grass_top"], drawCount);
        break;
    case WATER:
        RenderCube(multipleTextureID["water"].second[water_state], drawCount);
        break;
    case Tree_birch:
        RenderFace(textureID["tree_birch_top"], TOP, drawCount);
        RenderFace(textureID["tree_birch_top"], BOTTOM, drawCount);
        RenderFace(textureID["tree_birch_side"], LEFT, drawCount);
        RenderFace(textureID["tree_birch_side"], RIGHT, drawCount);
        RenderFace(textureID["tree_birch_side"], FRONT, drawCount);
        RenderFace(textureID["tree_birch_side"], BACK, drawCount);
        break;
    case Tree_oak:
        RenderFace(textureID["tree_oak_top"], TOP, drawCount);
        RenderFace(textureID["tree_oak_top"], BOTTOM, drawCount);
        RenderFace(textureID["tree_oak_side"], LEFT, drawCount);
        RenderFace(textureID["tree_oak_side"], RIGHT, drawCount);
        RenderFace(textureID["tree_oak_side"], FRONT, drawCount);
        RenderFace(textureID["tree_oak_side"], BACK, drawCount);
        break;
    case Tree_jungle:
        RenderFace(textureID["tree_jungle_top"], TOP, drawCount);
        RenderFace(textureID["tree_jungle_top"], BOTTOM, drawCount);
        RenderFace(textureID["tree_jungle_side"], LEFT, drawCount);
        RenderFace(textureID["tree_jungle_side"], RIGHT, drawCount);
        RenderFace(textureID["tree_jungle_side"], FRONT, drawCount);
        RenderFace(textureID["tree_jungle_side"], BACK, drawCount);
        break;
    case Leave_birch:
        RenderCube(textureID["leave_birch"], drawCount);
        break;
    case Leave_jungle:
        RenderCube(textureID["leave_jungle"], drawCount);
        break;
    case Leave_oak:
        RenderCube(textureID["leave_oak"], drawCount);
        break;
    case MUSHROOM:
        RenderFace(textureID["mushroom"],CROSS, drawCount);
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
    if (frame <= 10) {
        frame++;
    }
    else {
        frame = 0;
        water_state = (water_state + 1) % 32;
    }

}