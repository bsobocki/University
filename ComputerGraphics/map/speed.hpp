#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Speed
{
public:
   Speed() : 
      deltaTime(0.0f), 
      lastFrame(0.0f)
   {}

   void updateTime()
   {
      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
   }

   float deltaTime;
   float lastFrame;
};