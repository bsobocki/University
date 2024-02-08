#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#include "shader.hpp"
#include "camera.hpp"
#include "settings.hpp"
#include "speed.hpp"
#include "model.hpp"
#include "bubble.hpp"

#define RENDER_LOOP while(!glfwWindowShouldClose(window))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);

GLFWwindow * init();
void processInput(GLFWwindow *window);

void setShaderUniforms(Shader& shader, const glm::vec3& translationVec, float scale);
void setLightsInShader(Shader& shader, const glm::vec3& bubbleColor, size_t count, const glm::vec3* lightPos);
void setAquariumShaderUniforms(Shader& shader, const glm::vec3& scale);
void setProjectionAndView(Shader & shader);
void setShaderTransformationUniforms(Shader & shader, const glm::vec3& translationVec, float scale);

bool inAquarium(const glm::vec3 position);

float randomValue(float start, float end);
float randomColorValue();
glm::vec3 randomColor();
glm::vec3 randomBubblePosition();

Camera camera(glm::vec3(AQUARIUM_WIDTH/2, 10.0f, -300.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

Speed speed;

int main(int argc, char** argv)
{
    auto window = init();
    if (not window)
        return -1;

    Shader bubbleShader(bubbleVertexShaderPath, bubbleFragmentShaderPath);
    Shader ligthShader(ligthVertexShaderPath, ligthFragmentShaderPath);
    Shader aquariumShader(aquariumVertexShaderPath, aquariumFragmentShaderPath);

    Model bubble(bubbleModelPath);
    Model ligth(bubbleModelPath);
    Model aquarium(ligthModelPath);

    std::vector<bubble_info> bubble_infos;

    bubble_infos.push_back({
        randomColor(),
        randomBubblePosition(),
        bubbleScale
    });

    BubblesOrderer orderer(camera, bubble_infos);

    float waitingSart = glfwGetTime();
    float currTime = waitingSart;
    float timeToNextBubbleGeneration = randomValue(0.0, 1.0);

    RENDER_LOOP
    {
        speed.updateTime();
        processInput(window);

        float time = glfwGetTime()*5;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setProjectionAndView(ligthShader);
        setProjectionAndView(bubbleShader);
        
        glEnable(GL_CULL_FACE); 
        glCullFace(GL_BACK);  // włączam usuwanie plecóœ - pomaga przy transparentnych bańkach
        glFrontFace(GL_CCW);

        // lights moves
        for (int i=0; i<lightPositions.size(); i++)
        {
            glm::vec3 & pos = lightPositions[i];
            glm::vec3 & posMove = lightPositionsMove[i];
            if (pos.x < 0 || pos.x >= AQUARIUM_WIDTH)
                posMove.x *= -1;
            if (pos.z < -AQUARIUM_LENGTH || pos.z > AQUARIUM_LENGTH)
                posMove.z *= -1;

            pos.x += posMove.x * speed.deltaTime;
            pos.z += posMove.z * speed.deltaTime;
        }

        setLightsInShader(bubbleShader, glm::vec3(1.0), lightPositions.size(), &lightPositions[0]);

        // draw glowing cubes
        for (int i=0; i<lightPositions.size(); i++) {
            setShaderTransformationUniforms(ligthShader, lightPositions[i], ligthScale);
            ligth.Draw(ligthShader);
        }

        // add bubble if possible
        currTime = glfwGetTime();
        if (currTime - waitingSart > timeToNextBubbleGeneration && bubble_infos.size() < bubbles_num)
        {
            bubble_infos.push_back({
                randomColor(),
                randomBubblePosition(),
                randomValue(5.0, 30.0)
            });

            waitingSart = currTime;
            timeToNextBubbleGeneration = randomValue(0.5, 1.5);
        }

        // draw bubbles in order
        glDepthMask(GL_FALSE);

        auto order = orderer.getBubblesOrder();
        for(auto idx : order) 
        {
            auto & bInf = bubble_infos[idx];
            
            // increase bubble size
            if (bInf.scale < maxBubbleScale)
                bInf.scale += speed.deltaTime;

            // bigger bubble -> faster bubble
            bInf.position.y += speed.deltaTime * bInf.scale * 2;

            // if bubble came to aquarium edge, reload it on the bottom
            if(bInf.position.y >= AQUARIUM_HEIGHT)
            {
                bInf.position = randomBubblePosition();
                bInf.scale = bubbleScale;
            }

            // render
            bubbleShader.setVec3("bubbleColor", bInf.color);
            setShaderTransformationUniforms(bubbleShader, bInf.position, bInf.scale);
            bubble.Draw(bubbleShader);
        }

        glDepthMask(GL_TRUE);

        // aquarium
        // ------------
        // tutaj bedzie kod renderowania akwarium, ale to coś mi nie działa niestety :(

        setAquariumShaderUniforms(aquariumShader, AQUARIUM_DIMS);
        aquarium.Draw(aquariumShader);
        aquariumShader.setInt("isLine", 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        aquarium.Draw(aquariumShader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // -----------

        glfwSwapBuffers(window);
        glfwPollEvents();
}

   return 0;
}

float randomValue(float start, float end)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(start, end);
    return dist(e2);
}

float randomColorValue()
{
    return randomValue(0.3, 0.8);
}

glm::vec3 randomBubblePosition()
{
    return {randomValue(20.0, AQUARIUM_WIDTH), 0.0, randomValue(20.0, AQUARIUM_LENGTH)};
}

glm::vec3 randomColor()
{
    return {randomColorValue(), randomColorValue(), randomColorValue()};
}

void setLightsInShader(Shader& shader, const glm::vec3& bubbleColor, size_t count, const glm::vec3* lightPos)
{
    shader.use();

    shader.setVec3Arr("lightPos", lightPos, count);
    shader.setVec3("bubbleColor", bubbleColor);
    shader.setInt("lights_num", count);
}

void setShaderTransformationUniforms(Shader & shader, const glm::vec3& translationVec, float scale)
{
    shader.use();
        
    auto modelMat = IDENTITY_MATRIX;
    modelMat = glm::rotate(modelMat, rotationAngle, glm::vec3(0.0, 1.0, 0.0));
    modelMat = glm::translate(modelMat, translationVec);
    modelMat = glm::scale(modelMat, glm::vec3(scale, scale, scale));
    shader.setMat4("model", modelMat);
    shader.setVec3("spherePos", translationVec);
}

void setProjectionAndView(Shader & shader)
{
    shader.use();

    glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
    shader.setMat4("projection", projectionMat);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setVec3("viewPos", camera.getPosition());
}

void setAquariumShaderUniforms(Shader& shader, const glm::vec3& scale)
{
    shader.use();
        
    auto modelMat = IDENTITY_MATRIX;
    modelMat = glm::translate(modelMat, glm::vec3(scale.x/2, scale.y/2, scale.z/2));
    modelMat = glm::scale(modelMat, scale);

    glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

    shader.setMat4("model", modelMat);
    shader.setMat4("projection", projectionMat);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setVec3("viewPos", camera.getPosition());
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


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}