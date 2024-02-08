#version 330 core
uniform vec3 lightDir;
uniform vec3 viewPos;

in vec3 col;
in vec3 normal;
in vec3 fragPos;

const vec3 lightColor = vec3(1.0, 1.0, 1.0);

out vec4 color;

void main()
{
   vec3 bubColor = vec3(0.91, 0.82, 0.95);

   // diffuse
   float diff = max(dot(normal, vec3(0.0, 1.0, 1.0)), 0);
   vec3 diffuse = diff * lightColor * bubColor;

   color = vec4(diffuse, 0.1);
   //color = vec4(normal, 1.0);
}