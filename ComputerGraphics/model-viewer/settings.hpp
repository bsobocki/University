#define IDENTITY_MATRIX glm::mat4(1.0f)

const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;

const char* qubeVertexShaderPath = "shaders/qube.vs";
const char* qubeFragmentShaderPath = "shaders/qube.fs";
const char* whiteColorVertexShaderPath = "shaders/object.vs";
const char* whiteColorFragmentShaderPath = "shaders/object.fs";
const char* proceduralVertexShaderPath = "shaders/procedural.vs";
const char* proceduralFragmentShaderPath = "shaders/procedural.fs";
const char* modelVertexShaderPath = "shaders/model.vs";
const char* modelFragmentShaderPath = "shaders/model.fs";

const float maxCoord = 20.0f;

enum shaderSelection {SHADER_WHITE, SHADER_PROCEDURAL_TEXTURE, SHADER_IMAGE_TEXTURE};

int useTexture = 1;
int polygonMode = GL_FILL;
float rotationAngle = 0.0f;
float rotateVal = 0.0025f;
auto usedShader = shaderSelection::SHADER_IMAGE_TEXTURE;
float procTexMultiplier = 1.2;