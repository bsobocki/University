#include <iostream>
#include <fstream>
#include <iterator>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "camera.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>

#define IDENTITY_MATRIX glm::mat4(1.0f)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow *window);

struct object
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normal_vectors;
};

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

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

std::vector<glm::vec3> LoadFromObjectFile(std::string sFilename)
{
    std::ifstream f(sFilename);

    // Local cache of verts
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> objectData;

    char line[128];
    char junk;

    while (!f.eof())
    {
        f.getline(line, 128);

        std::stringstream s;
        s << line;

        if (line[0] == 'v')
        {
            glm::vec3 v;
            s >> junk >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }

        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            auto v1 = vertices[f[0]-1];
            auto v2 = vertices[f[1]-1];
            auto v3 = vertices[f[2]-1];
            auto normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            objectData.push_back(v1);
            objectData.push_back(normal);
            objectData.push_back(v2);
            objectData.push_back(normal);
            objectData.push_back(v3);
            objectData.push_back(normal);
        }
    }

    return objectData;
}

Camera camera(glm::vec3(0.0f, 0.0f, -0.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 sunLightDirection = glm::vec3(0.0f, 0.0f, -1.0f);

int main()
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
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glewExperimental=true;
   glewInit();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // aquarium
   unsigned int vertexBufferObjectID, vertexArrayObjectId;
   glGenVertexArrays(1, &vertexArrayObjectId);
   glGenBuffers(1, &vertexBufferObjectID);
   glBindVertexArray(vertexArrayObjectId);

   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

    // bubble
   auto bubbleObjectData = LoadFromObjectFile("sphere.obj");

   unsigned int bubbleVBO, bubbleVAO;
   glGenVertexArrays(1, &bubbleVAO);
   glGenBuffers(1, &bubbleVBO);

   glBindVertexArray(bubbleVAO);

   glBindBuffer(GL_ARRAY_BUFFER, bubbleVBO);
   glBufferData(GL_ARRAY_BUFFER, bubbleObjectData.size() * sizeof(glm::vec3), &bubbleObjectData[0], GL_STATIC_DRAW);

   // object data vertex: 3 first floats from data line
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   // object data normal: 3 next floats from data line
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   auto aquariumProgram = loadShaders("QubeVertexShader.vert","QubeFragmentShader.frag");
   auto bubbleProgram = loadShaders("BubbleVertexShader.vert","BubbleFragmentShader.frag");

    // aquarium
   glUseProgram(aquariumProgram);

   auto model = glm::translate(IDENTITY_MATRIX, glm::vec3(1.0f, 1.0f, -15.0f));
   model = glm::scale(model, glm::vec3(10.0f, 15.0f, 30.0f));

   auto projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);

   auto modelLocation = glGetUniformLocation(aquariumProgram, "model");
   auto viewLocation = glGetUniformLocation(aquariumProgram, "view");
   auto projectionLocation = glGetUniformLocation(aquariumProgram, "projection");
   glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
   
   // bubble
   glUseProgram(bubbleProgram);
   auto bubbleModel = IDENTITY_MATRIX;
   auto bubbleModelLocation = glGetUniformLocation(bubbleProgram, "model");
   auto bubbleViewLocation = glGetUniformLocation(bubbleProgram, "view");
   auto bubbleProjectionLocation = glGetUniformLocation(bubbleProgram, "projection");
   auto bubbleSunLightDirectionLocation = glGetUniformLocation(bubbleProgram, "lightDir");
   auto bubbleCameraPositionLocation = glGetUniformLocation(bubbleProgram, "lightPos");

   glUniformMatrix4fv(bubbleProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
   glUniform3fv(bubbleSunLightDirectionLocation, 1, &sunLightDirection[0]);
   glUniform3fv(bubbleSunLightDirectionLocation, 1, &camera.Position[0]);

    // -----RENDER--LOOP------
   while (!glfwWindowShouldClose(window))
   {
      // time for set camera speed
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // input
      // -----
      processInput(window);

      // render
      // ------
      glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      auto view = camera.GetViewMatrix();

      // for transparent objects
      glDepthMask(GL_FALSE);

      // bubble
      glUseProgram(bubbleProgram);
      glBindVertexArray(bubbleVAO);

      glUniformMatrix4fv(bubbleViewLocation, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(bubbleModelLocation, 1, GL_FALSE, glm::value_ptr(bubbleModel));

       // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       // glDrawElements(GL_TRIANGLES, bubbleIndices.size() * 3, GL_UNSIGNED_INT, 0);
       // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, bubbleObjectData.size() / 2 * 3);


      // aquarium
      glUseProgram(aquariumProgram);
      glBindVertexArray(vertexArrayObjectId);

      glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

      // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glDepthMask(GL_TRUE);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
   glfwTerminate();
   return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // make sure the viewport matches the new window dimensions; note that width and 
   // height will be significantly larger than specified on retina displays.
   glViewport(0, 0, width, height);
}