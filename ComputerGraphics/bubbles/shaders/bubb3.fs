#version 330 core
#define MAX_LIGHT_NUM 2
// //in vec3 col;
in vec3 FragPos;  
in vec3 Normal;
// //uniform vec3 lightPos;
out vec4 FragColor;
uniform vec3 viewPos;
uniform int lights_num;
uniform vec3 lightPos[MAX_LIGHT_NUM];

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 objectColor = vec3(1.0, 0.4, 0.85);
//vec3 lightPos = vec3(10.0, 10.0, -40.0);
float specularStrength = 0.9;
float maxVisibleDist = 1000.f;

// vec4 lightedWhiteColor(vec3 lightPos, float alpha)
// {


//    float ambientStrength = 0.3;
//    vec3 ambient = ambientStrength * lightColor;

//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);  
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor* objectColor;

//    vec3 result = (ambient + diffuse) ;
//    return vec4(result, alpha*diff/2);
// }

// vec4 light(vec3 lightPos, float alpha)
// {
//    vec3 lightPos1 = lightPos;
//    vec3 lightPos2 = vec3(lightPos.x, -lightPos.y, lightPos.z);
//    vec3 lightPos3 = vec3(lightPos.x, -lightPos.y, -lightPos.z);

//    vec4 color = lightedWhiteColor(lightPos1, alpha);
//    color += lightedWhiteColor(lightPos2, alpha);
//    color += lightedWhiteColor(lightPos3, alpha);
//    return color;
// }

// void main()
// {
//    vec3 lightPos1 = vec3(10.0, 42.0, 2.0);
//    float alpha = 0.7;
//    color = light(lightPos1, alpha);
// }

vec4 getLightColor(vec3 lightPosition)
{
   float alpha;
   float ambientStrength = 0.3;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 distVec = lightPosition - FragPos;
   float dist = length(distVec);
   vec3 lightDir = normalize(distVec);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
   vec3 specular = specularStrength * spec * lightColor; 
   vec3 result = (ambient + diffuse + specular) * objectColor * (1.0 - (dist/maxVisibleDist));
   return vec4(result, dot(result,result)/10);
}

void main()
{
   float wsp = 7.0;
   vec4 result = vec4(0.0);
   result += getLightColor(lightPos[0]);
   result += getLightColor(-lightPos[0]);
   result += getLightColor(lightPos[1]);
   result += getLightColor(-lightPos[1]);
   FragColor = vec4(result.xyz * objectColor, result.w);
}  