#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <stdlib.h>
#include <time.h>

#pragma comment (lib, "assimp-vc140-mt.lib")

using namespace std;
/**
* @brief ��������ڳ�ʼ��GLFWwindow
*/



class initialWindow {
public:

    /**
    * @brief ��Ҫ��������ʼ������
    * @param window ��ʼ���󷵻صĴ���ָ��
    * @param windowsWidth ����Ĵ��ڿ��
    * @param windowsHiehgt ����Ĵ��ڸ߶�
    * @param head ���ڵı���
    */
    static int initial(GLFWwindow* &window, int windowsWidth, int windowsHeight, const char* head) {
        // ����GLFW�ĳ�ʼ��
        // OpenGL�汾4.5
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // ���д��ڶ���Ĵ�������СΪ1920*1080
        window = glfwCreateWindow(windowsWidth, windowsHeight, head, NULL, NULL);
        if (window == NULL)
        {
            cout << "Fail to create GLFW window!" << endl;
            glfwTerminate();
            return -1;
        }
		// Set OpenGL options
		

		
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, getFrameBufferSize);
        glfwSetCursorPosCallback(window, mouse_callback);
      /*  glfwSetScrollCallback(window, scroll_callback);*/
        //��GLAD���г�ʼ��
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "Failed to initialize GLAD" << endl;
            Sleep(100);
            return -1;
        }
        return 0;
    }

private:
    /**
    * @brief ��鴰�ڵĴ�С�Ƿ�ı�
    * @param window ����Ĵ���ָ��
    * @param windowsWidth ����Ĵ��ڿ��
    * @param windowsHiehgt ����Ĵ��ڸ߶�
    */
    static void getFrameBufferSize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        xpos *= 2.0f;
        ypos *= 2.0f;
        if (Camera::getInstance()->firstMouse)
        {
            Camera::getInstance()->lastX = xpos;
            Camera::getInstance()->lastY = ypos;
            Camera::getInstance()->firstMouse = false;
        }

        float xoffset = xpos - Camera::getInstance()->lastX;
        float yoffset = Camera::getInstance()->lastY - ypos;
        Camera::getInstance()->lastX = xpos;
        Camera::getInstance()->lastY = ypos;
        Camera::getInstance()->lookAround(xoffset, yoffset);
    }
    
    ///**
    //* @brief ͨ�����ֽ�����ͼ�Ŵ���С
    //* @param window ����Ĵ���ָ��
    //* @param xoffset x�����ƫ����
    //* @param yoffset y�����ƫ����
    //*/
    //static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    //{
    //    // ��С
    //    if (Camera::getInstance()->fov >= 1.0f && Camera::getInstance()->fov <= 45.0f)
    //        Camera::getInstance()->fov -= yoffset;
    //    // �������
    //    if (Camera::getInstance()->fov <= 1.0f)
    //        Camera::getInstance()->fov = 1.0f;
    //    if (Camera::getInstance()->fov >= 45.0f)
    //        Camera::getInstance()->fov = 45.0f;
    //}
};