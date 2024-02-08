#include "camera.hpp"

#define PI 3.1415927f

float radians(float degries)
{
   return degries * PI/ 180.0f;
}

// constructor with vectors
Camera::Camera(glm::vec2 position, glm::vec3 up, float yaw, float pitch) : 
   Front(glm::vec3(0.2f, 0.1f, -1.0f)), 
   MovementSpeed(SPEED), 
   MouseSensitivity(SENSITIVITY), 
   Zoom(ZOOM)
{
   radius = 7000.0f;
   horizontalAngle = position.x;
   verticalAngle = position.y;
   WorldUp = up;
   Yaw = yaw;
   Pitch = pitch;
   updateCameraVectors();
   calcPosition();
}

void Camera::calcPosition()
{
   float x = cos(radians(verticalAngle)) * sin(radians(horizontalAngle)) * radius;
   float y = sin(radians(verticalAngle)) * radius;
   float z = cos(radians(verticalAngle)) * cos(radians(horizontalAngle)) * radius;
   Position = glm::vec3(x,y,z);
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : 
   Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
   MovementSpeed(SPEED), 
   MouseSensitivity(SENSITIVITY), 
   Zoom(ZOOM)
{
   Position = glm::vec3(posX, posY, posZ);
   WorldUp = glm::vec3(upX, upY, upZ);
   Yaw = yaw;
   Pitch = pitch;
   updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
   return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
   float velocity = MovementSpeed * deltaTime;
   if (direction == FORWARD)
      // Position += Front * velocity;
      radius -= velocity * 60;
   if (direction == BACKWARD)
      // Position -= Front * velocity;
      radius += velocity * 60;
   if (direction == LEFT)
      // Position -= Right * velocity;
      horizontalAngle -= velocity;
   if (direction == RIGHT)
      // Position += Right * velocity;
      horizontalAngle += velocity;
   if (direction == UP)
      // Position += Up * velocity;
      verticalAngle += velocity;
   if (direction == DOWN)
      // Position -= Up * velocity;
      verticalAngle -= velocity;
   calcPosition();
}

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
   xoffset *= MouseSensitivity;
   yoffset *= MouseSensitivity;

   Yaw   += xoffset;
   Pitch += yoffset;

   // make sure that when pitch is out of bounds, screen doesn't get flipped
   if (constrainPitch)
   {
      if (Pitch > 89.0f)
            Pitch = 89.0f;
      if (Pitch < -89.0f)
            Pitch = -89.0f;
   }

   // update Front, Right and Up Vectors using the updated Euler angles
   updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
   Zoom -= (float)yoffset;
   if (Zoom < 1.0f)
      Zoom = 1.0f;
   if (Zoom > 45.0f)
      Zoom = 45.0f; 
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
   // calculate the new Front vector
   glm::vec3 front;
   front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
   front.y = sin(glm::radians(Pitch));
   front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

   Front = glm::normalize(front);
   Right = glm::normalize(glm::cross(Front, WorldUp));
   Up    = glm::normalize(glm::cross(Right, Front));
}