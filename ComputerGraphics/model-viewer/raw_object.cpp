#include "raw_object.hpp"

namespace 
{
   GLuint slot = 0;
}

Object::Object(): trianglesCount(0) 
{}

bool Object::loadObject(const std::string& filePath)
{
   std::vector< unsigned int > vertexIndices;
   std::vector< unsigned int > uvIndices;
   std::vector< unsigned int > normalIndices;
   std::vector<glm::vec3> temp_vertices;
   std::vector<glm::vec2> temp_uvs;
   std::vector<glm::vec3> temp_normals;

   FILE * file = fopen(filePath.c_str(), "r");
   if( file == NULL )
   {
      printf("Impossible to open the file !\n");
      return false;
   }

   int res;
   char lineHeader[128];

   while( (res = fscanf(file, "%s", lineHeader)) != EOF )
   {
      if ( strcmp( lineHeader, "v" ) == 0 )
      {
         glm::vec3 vertex;
         fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
         temp_vertices.push_back(vertex);
      }
      else if ( strcmp( lineHeader, "vt" ) == 0 )
      {
         glm::vec2 uv;
         fscanf(file, "%f %f\n", &uv.x, &uv.y );
         temp_uvs.push_back(uv);
      }
      else if ( strcmp( lineHeader, "vn" ) == 0 )
      {
         glm::vec3 normal;
         fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
         temp_normals.push_back(normal);
      }
      else if ( strcmp( lineHeader, "f" ) == 0 )
      {
         std::string vertex1, vertex2, vertex3;
         unsigned int a;
         unsigned int vertexIndex[3], normalIndex[3];
         int matches = fscanf(file, "%u//%u %u//%u %u//%u\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
         if (matches != 6)
         {
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            //return false;
         }
         else
         {
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            trianglesCount++;
         }
      }

      for( unsigned int i=0; i<vertexIndices.size(); i++ )
      {
         unsigned int vertexIndex = vertexIndices[i];
         unsigned int normalIndex = normalIndices[i];

         auto vert = temp_vertices[ vertexIndex-1 ];
         vertices.push_back(vert);
         data.push_back(vert.x);
         data.push_back(vert.y);
         data.push_back(vert.z);

         auto norm = temp_normals[ normalIndex-1 ];
         normals.push_back(norm);
         data.push_back(norm.x);
         data.push_back(norm.y);
         data.push_back(norm.z);
      }
   }

   trianglesCount = (data.size()/2)/3;
   return true;
}

void Object::loadGraphicObj()
{
   GLuint vArrObjId;
   glGenVertexArrays(1, &vArrObjId);
   glBindVertexArray(vArrObjId);

   GLuint vBuffObjId;
   glGenBuffers(1, &vBuffObjId);
   glBindBuffer(GL_ARRAY_BUFFER, vBuffObjId);

   glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);

   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
   glEnableVertexAttribArray(1);
}

void Object::loadVertices(float* d, size_t size)
{
   for(int i=0; i<size; i++) data.push_back(d[i]);
   trianglesCount = (size/2)/3;
}

void Object::draw()
{
   glDrawArrays(GL_TRIANGLES, 0, trianglesCount);
}

void loadQubeVertices(Object & obj)
{
   float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,      0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,      0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,      0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,      0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,      0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,     0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,      0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,      0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,      0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,      0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,      0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,      0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,      0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,      0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,     0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f
   };

   obj.loadVertices(vertices, sizeof(vertices)/sizeof(float));
}