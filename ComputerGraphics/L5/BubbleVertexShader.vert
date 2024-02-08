#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 col;
out vec3 normal;
out vec3 fragPos;

void main()
{
   col = normalize(gl_Position.xyz);
   normal = inNormal;
   fragPos = vec3(model * vec4(position, 1.0)); // position of vetrex in world space (outside model space)
   gl_Position = projection * view * model * vec4(position, 1.0);
}