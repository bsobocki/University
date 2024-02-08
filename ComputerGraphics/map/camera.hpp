#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         =   90.0f; // obrót prawo-lewo
const float PITCH       =    0.0f; // obrot gora-dol
const float SPEED       =    2.0f;
const float SENSITIVITY =  0.065f;
const float ZOOM        =   45.0f;

class Camera
{
public:
   Camera(
      glm::vec2 position = glm::vec2(0.0f, 0.0f), 
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
      float yaw = YAW, 
      float pitch = PITCH);
   Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
   glm::mat4 GetViewMatrix();
   void ProcessKeyboard(Camera_Movement direction, float deltaTime);
   void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
   void ProcessMouseScroll(float yoffset);
   glm::vec3 getPosition() { return Position; }

   // public camera options
   float Zoom;
    float MovementSpeed;

private:
   void updateCameraVectors();
   void calcPosition();

    // camera Attributes
    float verticalAngle;
    float horizontalAngle;
    float radius;
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MouseSensitivity;
};

#endif