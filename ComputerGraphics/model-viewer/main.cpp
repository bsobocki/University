#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.hpp"
#include "camera.hpp"
#include "settings.hpp"
#include "speed.hpp"
#include "model.hpp"

#define RENDER_LOOP while(!glfwWindowShouldClose(window))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float getScaleToFitInScreen(Model & model);
void processInput(GLFWwindow *window);
GLFWwindow * init();

Camera camera(glm::vec3(00.0f, 10.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

Speed speed;

int main(int argc, char** argv)
{
    std::string objectPath = "objects/backpack2/backpack.obj";

    if (argc == 2)
        objectPath = argv[1];

   auto window = init();
   if (not window)
      return -1;

   std::vector<Shader> shaders;
   shaders.emplace_back(whiteColorVertexShaderPath, whiteColorFragmentShaderPath);
   shaders.emplace_back(proceduralVertexShaderPath, proceduralFragmentShaderPath);
   shaders.emplace_back(modelVertexShaderPath, modelFragmentShaderPath);

   Model model(objectPath);
   auto scale = getScaleToFitInScreen(model);

    RENDER_LOOP
    {
        speed.updateTime();
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader & shader = shaders[usedShader];
        shader.use();
        
        auto viewMat = camera.GetViewMatrix();
        glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        shader.setMat4("projection", projectionMat);
        shader.setMat4("view", viewMat);
        shader.setVec3("lightPos", camera.getPosition());
        shader.setInt("textureMode", useTexture);
        shader.setFloat("time", glfwGetTime()/10.0);
        shader.setFloat("procTexMultiplier", procTexMultiplier);

        // render the loaded model
        auto modelMat = IDENTITY_MATRIX;
        modelMat = glm::rotate(modelMat, rotationAngle, glm::vec3(0.0, 1.0, 0.0));
        modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));
        shader.setMat4("model", modelMat);
        model.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
}

   return 0;
}

float getScaleToFitInScreen(Model & model)
{
    auto modelMaxCoords = model.getMaxCoords();
    float scale = maxCoord/std::max(modelMaxCoords.x, modelMaxCoords.y);
    return scale;
}

GLFWwindow * init()
{
   glewExperimental=true;
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return window;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glewExperimental=true;
   glewInit();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   stbi_set_flip_vertically_on_load(true);

   return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    static float lastPolygonModeChangeTime = glfwGetTime();
    static float wasLeftShiftPressed = false;
    static float wasRightShiftPressed = false;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        float currTime = glfwGetTime();
        if (currTime - lastPolygonModeChangeTime > 1.0f) 
        {
            polygonMode = polygonMode == GL_LINE ? GL_FILL : GL_LINE;
            glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
            lastPolygonModeChangeTime = currTime;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        rotationAngle += rotateVal;
        if (rotationAngle >= 6.2832f)
            rotationAngle -= 6.2832f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        rotationAngle -= rotateVal;
        if (rotationAngle < 0)
            rotationAngle += 6.2832f;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        procTexMultiplier += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        procTexMultiplier -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        usedShader = SHADER_WHITE;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        useTexture = 2;
        usedShader = SHADER_PROCEDURAL_TEXTURE;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        useTexture = 0;
        usedShader = SHADER_PROCEDURAL_TEXTURE;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        useTexture = 1;
        usedShader = SHADER_PROCEDURAL_TEXTURE;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        usedShader = SHADER_IMAGE_TEXTURE;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        wasLeftShiftPressed = true;
        camera.MovementSpeed = 4*SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        if (wasLeftShiftPressed) {
            camera.MovementSpeed = SPEED;
            wasLeftShiftPressed = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        wasRightShiftPressed = true;
        camera.MovementSpeed = 8*SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) {
        if (wasRightShiftPressed) {
            camera.MovementSpeed = SPEED;
            wasRightShiftPressed = false;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    static bool firstMouse = true;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}