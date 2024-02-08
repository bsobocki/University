#include "shader.hpp"

unsigned int loadShaders(const char * vertexPath, const char * fragmentPath);

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
   //ID = loadShaders(vertexPath, fragmentPath);
   auto code = readShaderCodeFromFile(vertexPath, fragmentPath);
   auto ids = compileShaderCode(code);
   initProgram(ids);

   glDeleteShader(ids.vertexId);
   glDeleteShader(ids.fragmentId);
}

shaderCode Shader::readShaderCodeFromFile(const char* vertexPath, const char* fragmentPath)
{
   std::stringstream vShaderStream;
   std::stringstream fShaderStream;

   try 
   {
      std::ifstream vShaderFile(vertexPath);
      std::ifstream fShaderFile(fragmentPath);

      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      vShaderStream << vShaderFile.rdbuf();
      vShaderFile.close();

      fShaderStream << fShaderFile.rdbuf();
      fShaderFile.close();
   }
   catch (std::ifstream::failure& e)
   {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
   }

   return {vShaderStream.str(), fShaderStream.str()};
}

shaderIds Shader::compileShaderCode(const shaderCode & code)
{
   const char* vShaderCode = code.vertexCode.c_str();
   unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex, 1, &vShaderCode, NULL);
   glCompileShader(vertex);
   checkCompileErrors(vertex, "VERTEX");

   const char* fShaderCode = code.fragmentCode.c_str();
   unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment, 1, &fShaderCode, NULL);
   glCompileShader(fragment);
   checkCompileErrors(fragment, "FRAGMENT");

   return {vertex, fragment};
}

void Shader::initProgram(const shaderIds& ids)
{
   ID = glCreateProgram();
   glAttachShader(ID, ids.vertexId);
   glAttachShader(ID, ids.fragmentId);
   glLinkProgram(ID);
   checkCompileErrors(ID, "PROGRAM");
}

void Shader::use() 
{ 
   glUseProgram(ID); 
}

void Shader::setBool(const std::string &name, bool value) const
{         
   glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
   glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
   glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setMat4(const std::string& name, const glm::highp_mat4& value) const
{
   glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
   glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3Arr(const std::string& name, const glm::vec3* value, const size_t count)
{
   glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(value[0]));
}

void Shader::setIntArr(const std::string& name, const int* value, const size_t count)
{
   glUniform3iv(glGetUniformLocation(ID, name.c_str()), count, value);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
   int success;
   constexpr size_t infoLogSize = 1024;
   char infoLog[infoLogSize];
   if (type != "PROGRAM")
   {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
            glGetShaderInfoLog(shader, infoLogSize, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
   }
   else
   {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
            glGetProgramInfoLog(shader, infoLogSize, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
   }
}

unsigned int loadShaders(const char * vertexPath, const char * fragmentPath)
{
   std::string vertexCode;
   std::string fragmentCode;
   std::ifstream vShaderFile(vertexPath);
   std::ifstream fShaderFile(fragmentPath);
   // ensure ifstream objects can throw exceptions:
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try
   {
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
   }
   catch (std::ifstream::failure& e)
   {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
   }
   const char* vertexShaderContext = vertexCode.c_str();
   const char* fragmentShaderContext = fragmentCode.c_str();

   unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShaderId, 1, &vertexShaderContext, NULL);
   glCompileShader(vertexShaderId);

   unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, &fragmentShaderContext, NULL);
   glCompileShader(fragmentShaderId);

   unsigned int shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShaderId);
   glAttachShader(shaderProgram, fragmentShaderId);
   glLinkProgram(shaderProgram);

   glDeleteShader(vertexShaderId);
   glDeleteShader(fragmentShaderId);

   return shaderProgram;
}