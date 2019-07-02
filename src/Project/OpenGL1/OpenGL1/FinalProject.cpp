#include "Initial.h"
#include "Shader.h"
#include <vector>
#include "Light.h"
#include "ResourceManager.h"
#include "World.h"
#include "ShaderManager.h"
#include "TextRender.h"
#include "SnowParticle.h"
// ************************�趨�õĲ���************************************
const unsigned int windowsWidth = 2560;
const unsigned int windowsHeight = 1440;
const char* head = "Final Project";
// ���ڼ�¼����ƶ�
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// �жϵ�ǰ����״̬������ѡ���Ƿ��Ƿ���ģʽ
bool isPress = false;
/**
* @brief ����Esc�˳�
* @param window ����Ĵ���ָ��
* @param deltaTime ��ӵ��²���
*/
void escapePress(GLFWwindow *window, float& deltaTime);

// ����ģʽָ��
ResourceManager* manager = ResourceManager::getInstance();
Camera* camera = Camera::getInstance();
World* world = World::getInstance();
ShaderManager* shaderManager = ShaderManager::getInsatance();
int main()
{
	// ��ʼ������
	GLFWwindow* window = NULL;
	if (initialWindow::initial(window, windowsWidth, windowsHeight, head) == -1) return -1;

	shaderManager->InitialShader();
	manager->setAllTexture();


	// Configure depth map FBO ����߿�1024
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	// Ϊ��Ⱦ�������ͼ����һ��֡�������
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// �����������
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��ֹ�������� ----------------------------------------
	// ��ֹ������ͼ��Զ���ظ���Ⱦ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	// �����ɵ����������Ϊ֡�������Ȼ���
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	manager->InitCube();
	// ��Ⱦ��պ�
	manager->InitSky();
	test_vao(windowsWidth, windowsHeight);

	SnowParticle::SnowParticle mSnow;

	while (!glfwWindowShouldClose(window))
	{


		// ��������
		escapePress(window, deltaTime);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
       /* glEnable(GL_FRAMEBUFFER_SRGB);*/

		 // ----------------��Ӱ����----------------------------------------------
		 // Ϊ���޸�peter���ƣ����������޳�
		glCullFace(GL_FRONT);

		glm::mat4 lightProjection(1.0f), lightView(1.0f), lightSpaceMatrix(1.0f);
		// ��Ϊ��Դʹ����������Ͷ��ͶӰ����
		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		// ͸��ͶӰ
		lightProjection = glm::perspective(100.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderManager->simpleDepthShader.use();
		shaderManager->simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//shaderManager->textshader.use();
		
		// ��Ⱦ�����ͼ
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		world->Render(shaderManager->simpleDepthShader);
		/*manager->RenderScene(simpleDepthShader, glm::vec3(1.0f, 3.0f, 2.0f), manager->GRASS);*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glCullFace(GL_BACK);


		// ���ʹ�������ͼ��Ⱦ
		glViewport(0, 0, windowsWidth, windowsHeight);

		// ������ɫ
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


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderManager->ObjectShaderSet(projection, view, lightPos, lightSpaceMatrix);
		// ��������
		world->Render(shaderManager->blockShader);

		glm::mat4 model_snow(1.0f);
		glm::mat4 projection_snow = glm::ortho(-15.0, 15.0, 0.0, 15.0, -3.0, 100.0);
		mSnow.Render(deltaTime, model_snow, view, projection_snow);
		
		char str[20];
		sprintf_s(str, "Score = %d", world->GetScore());
		RenderText(str, 60, 1000, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), windowsWidth, windowsHeight);
		char str_s[20];
		sprintf_s(str_s, "TIME: %0.2f s", glfwGetTime());
		RenderText(str_s, 300, 1000, 0.6f, glm::vec3(1.0f, 1.0f, 1.0f), windowsWidth, windowsHeight);
	   /* ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        */
		if (camera->Position.y < -5.0f) {
			char str_s[20];
			sprintf_s(str_s, "GAME OVER");
			RenderText(str_s, 500, windowsHeight/2, 2.5f, glm::vec3(1.0f, 1.0f, 1.0f), windowsWidth, windowsHeight);
		}
        // ��Ⱦ���
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
* @brief ���м��̰����ļ��
* @param window ����ָ��
* @param deltaTime ��������ʱ��
*/
void escapePress(GLFWwindow *window, float& deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// �������д�����������ٶ�
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		// WA
		// cout << "WA" << endl;
		camera->ProcessKeyboard(FORWARD_LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		// W
		// cout << "W" << endl;
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		// WD
		// cout << "WD" << endl;
		camera->ProcessKeyboard(FORWARD_RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		// SA
		// cout << "SA" << endl;
		camera->ProcessKeyboard(BACKWARD_LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		// S
		// cout << "S" << endl;
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		// SD
		// cout << "SD" << endl;
		camera->ProcessKeyboard(BACKWARD_RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		// A
		// cout << "A" << endl;
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		// D
		// cout << "D" << endl;
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		isPress = true;
	}
	else if (isPress && glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) {
		isPress = false;
		camera->ProcessKeyboard(FLYSKY, deltaTime);
	}
	// ��Ծ��������
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(JUMP, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera->ProcessKeyboard(DOWN, deltaTime);
	}
}