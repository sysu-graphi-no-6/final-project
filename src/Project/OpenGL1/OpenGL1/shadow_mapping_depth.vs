#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aOffset;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos.x + aOffset.x, aPos.y + aOffset.y + 1.0f, aPos.z + aOffset.z, 1.0f);
}