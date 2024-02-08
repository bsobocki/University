#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#define IDENTITY_MATRIX glm::mat4(1.0f)

#include "vertices_data.hpp"

const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;

const char* bubbleVertexShaderPath = "shaders/bubble.vs";
const char* bubbleFragmentShaderPath = "shaders/bubble.fs";
const char* ligthVertexShaderPath = "shaders/qube.vs";
const char* ligthFragmentShaderPath = "shaders/qube.fs";
const char* aquariumVertexShaderPath = "shaders/aquarium.vs";

// AQUARIUM
const char* aquariumLineFragmentShaderPath = "shaders/aquariumLine.fs";
const char* aquariumFragmentShaderPath = "shaders/aquarium.fs";
const float AQUARIUM_WIDTH = 400.0;
const float AQUARIUM_HEIGHT = 400.0;
const float AQUARIUM_LENGTH = 800.0;
glm::vec3 AQUARIUM_DIMS = {AQUARIUM_WIDTH, AQUARIUM_HEIGHT, AQUARIUM_LENGTH};

int polygonMode = GL_FILL;

float rotationAngle = 0.0f;
float rotateVal = 0.0025f;

float ligthScale = 4.0f;
 // cube lights
std::vector<glm::vec3> lightPositions 
{
   glm::vec3( 100.0, AQUARIUM_HEIGHT, 10.0),
   glm::vec3( 184.0, AQUARIUM_HEIGHT, 10.0),
   glm::vec3(  36.0, AQUARIUM_HEIGHT, 40.0),
   glm::vec3(0.0),
   glm::vec3(0.0, 0.0, AQUARIUM_LENGTH),
   glm::vec3(AQUARIUM_WIDTH, 0.0, 0.0),
   glm::vec3(AQUARIUM_WIDTH, 0.0, AQUARIUM_LENGTH),
};

// cube lights
std::vector<glm::vec3> lightPositionsMove 
{
   glm::vec3(30.0, 0.0, 50.0),
   glm::vec3(34.0, 0.0, 30.0),
   glm::vec3(20.0, 0.0, 20.0),
   glm::vec3(0.0),
   glm::vec3(0.0),
   glm::vec3(0.0),
   glm::vec3(0.0)
};

const float maxBubbleScale = 30.0;
float bubbleScale = 20.0f;
int bubbles_num = 20;

std::string bubbleModelPath = "objects/sphere.obj";
std::string ligthModelPath = "objects/qube.obj";

#endif