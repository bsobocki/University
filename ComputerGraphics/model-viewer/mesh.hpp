#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
   // position
   glm::vec3 Position;
   // normal
   glm::vec3 Normal;
   // texCoords
   glm::vec2 TexCoords;
   // tangent
   glm::vec3 Tangent;
   // bitangent
   glm::vec3 Bitangent;
   //bone indexes which will influence this vertex
   int m_BoneIDs[MAX_BONE_INFLUENCE];
   //weights from each bone
   float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
   unsigned int id;
   string type;
   string path;
};

class Mesh {
public:
   Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
   void Draw(Shader &shader);
   glm::vec3 getMaxCoords();

   vector<Vertex>  vertices;
   vector<GLuint>  indices;
   vector<Texture> textures;
   unsigned int VAO;

private:
   void setupMesh();

   // render data 
   unsigned int VBO, EBO;
};
#endif

