#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#include "tile.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "speed.hpp"
#include "sphere.hpp"
#include "HGTreader.hpp"

#include "definitions.hpp"
#include "settings.hpp"

Camera camera(glm::vec2(180.f + 6.0, 52.0));
Speed speed;
Tiles* tilesPtr = NULL;
readingInfo* riPtr = NULL;
HGTreader* readerPtr = NULL;
char* pathPtr;

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Za malo argumentow!\n";
        std::cout << "użycie: ./teren <katalog_z_plikami_hgt> [-n <stopnie polnocne [0;90]>] [-s <stopnie poludniowe [0:90]>] [-e <stopnie wschodnie [0;180]>] [-e <stopnie zachodnie [0;90]>]\n";
        return 1;
    }

    pathPtr = argv[1];
    argc--;
    
    readingInfo ri;
    riPtr = &ri;

    int arg=2;
    while(arg<argc-1)
    {
        std::string a = argv[arg];
        degreeInfo* di;

        if(std::string(argv[arg]) == "-n")
            di = &ri.n;
        if(std::string(argv[arg]) == "-s")
            di = &ri.s;
        if(std::string(argv[arg]) == "-e")
            di = &ri.e;
        if(std::string(argv[arg]) == "-w")
            di = &ri.w;

        di->mind = atoi(argv[arg+1]);
        di->maxd = atoi(argv[arg+2]);
        arg+=3;
    }

    auto window = init();
    if (not window)
        return -1;

    HGTreader reader;
    readerPtr = &reader;

    Shader tileShader("shaders/tile.vs", "shaders/tile.fs");
    Shader sphereShader("shaders/sphere.vs", "shaders/sphere.fs");
    Sphere sphere(sphereShader);
    Tiles tiles(tileShader);
    tilesPtr = &tiles;

    reader.readDataFromDirectory(pathPtr, ri, tiles.tiles, lod);

    float timeFromLastMeasure = 0.0;
    float currentMeasure;
    int rendersCounter = 0;

    RENDER_LOOP
    {
        currentMeasure = glfwGetTime();
        
        speed.updateTime();
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tileShader.setProjectionAndView(camera, SCR_WIDTH, SCR_HEIGHT);
        sphereShader.setProjectionAndView(camera, SCR_WIDTH, SCR_HEIGHT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        sphere.draw();

        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        tiles.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        float diff = currentMeasure - timeFromLastMeasure;
        rendersCounter++;

        if (diff > 1.0)
        {
            std::cout<<"fps = "<<float(rendersCounter)/diff<<std::endl;
            timeFromLastMeasure = currentMeasure;
            rendersCounter = 0;
        }
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

   return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    static float lastPolygonModeChangeTime = glfwGetTime();
    static float wasLeftShiftPressed = false;
    static float wasRightShiftPressed = false;
    static float wasLeftAltPressed = false;
    static float changedLOD = false;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, speed.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, speed.deltaTime);
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        wasLeftAltPressed = true;
        camera.MovementSpeed = SPEED/4;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
        if (wasLeftAltPressed) {
            camera.MovementSpeed = SPEED;
            wasRightShiftPressed = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_0)) {
        if (lod != 4) {
            lod = 4;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_1)) {
        if (lod != 8) {
            lod = 8;
            changedLOD = true;
        }
    }


    if (glfwGetKey(window, GLFW_KEY_2)) {
        if (lod != 12) {
            lod = 12;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_3)) {
        if (lod != 16) {
            lod = 16;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_4)) {
        if (lod != 20) {
            lod = 20;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_5)) {
        if (lod != 24) {
            lod = 24;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_6)) {
        if (lod != 30) {
            lod = 30;
            changedLOD = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_7)) {
        if (lod != 40) {
            lod = 40;
            changedLOD = true;
        }
    }

    // F -> full
    if (glfwGetKey(window, GLFW_KEY_F)) {
        if (lod != 1) {
            lod = 1;
            changedLOD = true;
        }
    }

    // F -> full
    if (glfwGetKey(window, GLFW_KEY_G)) {
        if (lod != 2) {
            lod = 2;
            changedLOD = true;
        }
    }

    if (changedLOD) {
        if (readerPtr and pathPtr and riPtr and tilesPtr) {
            tilesPtr->tiles.clear();
            tilesPtr->updateCurrIndicesIdx(lod);
            readerPtr->readDataFromDirectory(pathPtr, *riPtr, tilesPtr->tiles, lod);
        }
        changedLOD = false;
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