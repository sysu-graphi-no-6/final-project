#pragma once
#include "Initial.h"
// 光源位置
glm::vec3 lightPos(-2.0f, 4.0f, 10.0f);

// 定向光源
glm::vec3 direcionalDirection = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 directionalAmbient = glm::vec3(0.4f);
glm::vec3 directionalDiffuse = glm::vec3(0.6f);
glm::vec3 directionalSpecular = glm::vec3(0.1f);