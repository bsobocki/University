#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define IDENTITY_MATRIX glm::mat4(1.0f)
#define RENDER_LOOP while(!glfwWindowShouldClose(window))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

GLFWwindow * init();
void processInput(GLFWwindow *window);

std::vector<int> getIndices();
void setTileBuffers(std::vector<float>& data, std::vector<int>& indices);
void drawMap(Shader& shader, std::vector<int> indices);

#endif