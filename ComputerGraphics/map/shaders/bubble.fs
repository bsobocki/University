#version 330 core
// //in vec3 col;
in vec3 FragPos;  
in vec3 Normal;
// //uniform vec3 lightPos;
out vec4 FragColor;
uniform vec3 viewPos;
uniform int lights_num;
uniform vec3 lightPos[10];
uniform float time;
uniform vec3 spherePos;
uniform vec3 bubbleColor;

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 objectColor = vec3(0.99, 0.8, 0.8);
float ambientStrength = 0.02;
float specularStrength = 0.9;
float maxVisibleDist = 1000.f;

vec4 getLightColor(vec3 lightPosition)
{
   vec3 ambient = ambientStrength * lightColor;
   vec3 distVec = lightPosition - FragPos;
   float dist = length(distVec);
   float lightDistFactor = max(1 - dist/600, 0.0);

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPosition - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor ;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;
   vec3 result = (ambient + diffuse + specular) * objectColor * lightDistFactor;
   return vec4(result , dot(result,result) * 0.5);
}

vec4 getLights(vec3 lPos)
{
   vec3 distVec = spherePos - lPos;
   vec3 lPos2 = spherePos + distVec;
   return getLightColor(lPos) + getLightColor(lPos2);
}

#define lightPosition(i) getLights(lightPos[i])

void main()
{
   // swiatla z szescianow
   FragColor = vec4(0.0);
   for(int i=0; i<lights_num; i++) {
      FragColor += lightPosition(i);
   }

   // swiatlo sloneczne
   specularStrength = 0.5;
   ambientStrength = 0.2;
   float sunLightStrength = 0.7;
   lightColor = vec3(0.7, 0.6, 0.99); // różowe, by nadac banka różu
   // z gory banki
   vec3 sunLightPos = vec3(spherePos.x, spherePos.y + 100.0, spherePos.y);
   FragColor += getLights(sunLightPos) * sunLightStrength * vec4(objectColor, 1.0);
   // z lewa + gory
   sunLightPos = vec3(spherePos.x - 100, spherePos.y + 100.0, spherePos.y);
   FragColor += getLights(sunLightPos) * sunLightStrength  * vec4(objectColor, 1.0);
   // z prawa + gory
   sunLightPos = vec3(spherePos.x + 100, spherePos.y + 100.0, spherePos.y);
   FragColor += getLights(sunLightPos) * sunLightStrength * vec4(objectColor, 1.0);

   // nalozenie koloru banki
   FragColor *= vec4(bubbleColor, 1.0);

   vec3 viewDistVec = viewPos - spherePos;
   float viewDist = length(viewDistVec);
   float viewDistFactor = max(1 - viewDist/800, 0.0);

   // dodanie koloru kamery
   lightColor = vec3(0.98, 0.89, 0.2);
   FragColor += getLightColor(viewPos) * viewDistFactor;


   // wspolczynnik odleglosci banki od gracza - bardziej widoczne te blizej
   FragColor.w *= viewDistFactor;
}