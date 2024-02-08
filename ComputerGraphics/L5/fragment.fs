#version 330 core
// //in vec3 col;
in vec3 FragPos;  
in vec3 Normal;
// //uniform vec3 lightPos;
out vec4 FragColor;
uniform vec3 viewPos;

vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 objectColor = vec3(0.55, 0.2, 0.85);
vec3 lightPos = vec3(10.0, 10.0, -20.0);
float specularStrength = 1.2;

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

vec3 getLightColor(vec3 lightPosition)
{
   float alpha;
   float ambientStrength = 0.2;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPosition - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
   vec3 specular = specularStrength * spec * lightColor; 

   return (ambient + diffuse + specular) * objectColor;
}

void main()
{
   float wsp = 5.0;
   vec3 result = getLightColor(lightPos);
   result += getLightColor(-lightPos);
   result += getLightColor(-cross(lightPos, -lightPos));
   float colorVal = dot(result, result);
   //colorVal = colorVal * colorVal;
   // if (1 - colorVal > 0.2) {
   //     wsp = 1.5;
   // }
   FragColor = vec4(result + 0.2, colorVal/wsp + 0.05);
}  

// void main()
// {
//    float wsp = 10.0;
//    vec3 result = getLightColor(lightPos);
//    result += getLightColor(-lightPos);
//    float colorVal = dot(result, result);
//    colorVal = colorVal * colorVal;
//    // if (1 - colorVal > 0.2) {
//    //     wsp = 1.5;
//    // }
//    FragColor = vec4(result, colorVal/wsp);
// }  