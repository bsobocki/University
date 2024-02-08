#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "camera.hpp"

#define IDENTITY_MATRIX glm::mat4(1.0f)

struct shaderCode {
   const std::string vertexCode;
   const std::string fragmentCode;
};

struct shaderIds {
   const GLuint vertexId;
   const GLuint fragmentId;
};

class Shader
{
public:
   Shader(const char* vertexPath, const char* fragmentPath);
   void use();

   void setBool(const std::string& name, bool value) const;
   void setInt(const std::string& name, int value) const;
   void setFloat(const std::string& name, float value) const;
   void setMat4(const std::string& name, const glm::highp_mat4& value) const;
   void setVec3(const std::string& name, const glm::vec3& value);
   void setVec3Arr(const std::string& name, const glm::vec3* value, const size_t count);
   void setIntArr(const std::string& name, const int* value, const size_t count);
   void setShaderTransformationUniforms(const glm::vec3& translationVec, float scale);
   void setProjectionAndView(Camera& camera, const float scr_width, const float scr_height);

   GLuint ID;

private:
   void checkCompileErrors(unsigned int shader, std::string type);
   shaderCode readShaderCodeFromFile(const char* vertexPath, const char* fragmentPath);
   shaderIds compileShaderCode(const shaderCode& code);
   void initProgram(const shaderIds& ids);
};

#endif