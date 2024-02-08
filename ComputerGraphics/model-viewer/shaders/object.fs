#version 330 core
//in vec3 col;
in vec3 FragPos;  
in vec3 Normal;
uniform vec3 lightPos;
out vec4 color;

vec4 lightedWhiteColor()
{
   vec3 objectColor = vec3(0.85, 0.85, 0.85);
   vec3 lightColor = vec3(1.0, 1.0, 1.0);

   float ambientStrength = 0.07;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);  
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   vec3 result = (ambient + diffuse) * objectColor;
   return vec4(result, 1.0);
}

void main()
{
   color = lightedWhiteColor();
}