#ifndef LOADABLE_MODEL_H
#define LOADABLE_MODEL_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

class Object
{
public:
   Object();
   bool loadObject(const std::string& filePath);
   void loadVertices(float* data, size_t size);
   void loadGraphicObj();
   void draw();

   size_t trianglesCount;
   std::vector<glm::vec3> vertices;
   std::vector<glm::vec3> normals;
   std::vector<GLfloat> data;
};

void loadQubeVertices(Object & obj);
#endif