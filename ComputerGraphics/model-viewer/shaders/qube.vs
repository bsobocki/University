#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
out vec3 col;

void main()
{
   gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0);
   col = normalize((modelMat * vec4(position, 1.0)).xyz);
}