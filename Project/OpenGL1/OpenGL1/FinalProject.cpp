#include "Initial.h"
#include "Shader.h"
#include <vector>
#include "Light.h"
#include "ResourceManager.h"
#include "World.h"
#include "ShaderManager.h"
// ************************设定好的参数************************************
const unsigned int windowsWidth = 1920;
const unsigned int windowsHeight = 1080;
const char* head = "Final Project";
// 用于记录鼠标移动
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// 判断当前按键状态，用于选择是否是飞天模式
bool isPress = false;
/**
* @brief 按下Esc退出
* @param window 传入的窗口指针
* @param deltaTime 添加的新参数
*/
void escapePress(GLFWwindow *window, float& deltaTime);

// 单例模式指针
ResourceManager* manager = ResourceManager::getInstance();
Camera* camera = Camera::getInstance();
World* world = World::getInstance();
ShaderManager* shaderManager = ShaderManager::getInsatance();
int main()
{
    // 初始化窗口
    GLFWwindow* window = NULL;
    if (initialWindow::initial(window, windowsWidth, windowsHeight, head) == -1) return -1;

    /**
    * IMGUI
    */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    // 加载着色器
    //Shader shader("shadow.vs", "shadow.frag");
    //Shader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
    ////Shader lampShader("spotLight.vs", "spotLight.frag");
    //Shader directionalLight("multipleLight.vs", "multipleLight.frag");
    shaderManager->InitialShader();
    manager->setAllTexture();

    
    // 光源VAO
    /*unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/
    
    // Configure depth map FBO 纹理高宽1024
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    // 为渲染的深度贴图创建一个帧缓冲对象
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // 创建深度纹理
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 防止采样过多 ----------------------------------------
    // 防止纹理贴图在远处重复渲染
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


    // 把生成的深度纹理作为帧缓冲的深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    manager->InitCube();
    // 渲染天空盒
    manager->InitSky();

    while (!glfwWindowShouldClose(window))
    {

        // 按键控制
        escapePress(window, deltaTime);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // 设置平行光
        shaderManager->DirectionalLightSet();
        /**
        * 使用IMGUI
        */
       /* ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Attributes");
        ImGui::Text("Chooce projection type");
        ImGui::RadioButton("ortho type", &currentModel, orthoType);
        ImGui::RadioButton("perspective type", &currentModel, perspectiveType);

        ImGui::End();*/

        // ----------------阴影部分----------------------------------------------
        // 为了修复peter游移，进行正面剔除
        glCullFace(GL_FRONT);

        glm::mat4 lightProjection(1.0f), lightView(1.0f), lightSpaceMatrix(1.0f);
        // 将为光源使用正交或是投视投影矩阵
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        // 透视投影
        lightProjection = glm::perspective(100.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        //lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        shaderManager->simpleDepthShader.use();
        shaderManager->simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // 渲染深度贴图
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        world->Render(shaderManager->simpleDepthShader);
        /*manager->RenderScene(simpleDepthShader, glm::vec3(1.0f, 3.0f, 2.0f), manager->GRASS);*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCullFace(GL_BACK);


        // 这次使用深度贴图渲染
        glViewport(0, 0, windowsWidth, windowsHeight);

        // 背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        shaderManager->blockShader.use();
        glm::mat4 projection(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 model(1.0f);
        view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
        projection = glm::perspective(glm::radians(camera->Zoom), (float)windowsWidth / (float)windowsHeight, 0.1f, 100.0f);
        shaderManager->directionalLight.setMat4("projection", projection);
        shaderManager->directionalLight.setMat4("view", view);
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        shaderManager->ObjectShaderSet(projection, view, lightPos, lightSpaceMatrix);
        // 画出物体
        world->Render(shaderManager->blockShader);
        //// 画出光源
        //lampShader.use();
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        //directionalLight.setMat4("model", model);
        //lampShader.setMat4("projection", projection);
        //lampShader.setMat4("model", model);
        //lampShader.setMat4("view", view);

        //glBindVertexArray(lightVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

       /* ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
*/   
        // 渲染天空
        camera->UpdatePositionEachSecond(deltaTime);
        manager->RenderSky(projection);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //// Cleanup
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
/**
* @brief 进行键盘按键的检测
* @param window 窗口指针
* @param deltaTime 按键持续时间
*/
void escapePress(GLFWwindow *window, float& deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 单独进行处理提高运算速度
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // WA
        cout << "WA" << endl;
        camera->ProcessKeyboard(FORWARD_LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        // W
        cout << "W" << endl;
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // WD
        cout << "WD" << endl;
        camera->ProcessKeyboard(FORWARD_RIGHT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // SA
        cout << "SA" << endl;
        camera->ProcessKeyboard(BACKWARD_LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        // S
        cout << "S" << endl;
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // SD
        cout << "SD" << endl;
        camera->ProcessKeyboard(BACKWARD_RIGHT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        // A
        cout << "A" << endl;
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        // D
        cout << "D" << endl;
        camera->ProcessKeyboard(RIGHT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        isPress = true;
    }
    else if (isPress && glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) {
        isPress = false;
        camera->ProcessKeyboard(FLYSKY, deltaTime);
    }
    // 跳跃单独处理
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->ProcessKeyboard(JUMP, deltaTime);
}