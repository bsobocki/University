#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//out vec3 col;
out vec3 Normal;
out vec3 FragPos;

void main()
{
   gl_Position = projection * view * model * vec4(position, 1.0);
   FragPos = vec3(model * vec4(position, 1.0));
   //Normal = (model * vec4(normal, 1.0)).xyz; // for correct light model matrix transformations
   Normal = mat3(transpose(inverse(model))) * normal; 
}