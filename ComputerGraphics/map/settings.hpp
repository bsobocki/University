#ifndef SETTINGS_HPP
#define SETTINGS_HPP

const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;

int polygonMode = GL_FILL;

float rotationAngle = 0.0f;
float rotateVal = 0.0025f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

int lod = 8;

#endif