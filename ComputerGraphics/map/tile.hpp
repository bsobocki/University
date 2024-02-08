#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "shader.hpp"

#define MAX_TILE_SIZE 1201

const std::vector<int> supportedLods {1,2,4,8,12,16,20,24,30,40};

class Tile 
{
public:
   Tile(float _verticalAngle, float _horizontalAngle, std::vector<float>& _data, const int _lod):
      verticalAngle(_verticalAngle),
      horizontalAngle(_horizontalAngle)
   {
      lod = _lod;
      // z tym dziala -- nie usuwaj !!!
      if (lod==1)
         data = _data;
      else 
      {
         int data_size = MAX_TILE_SIZE;
         for(int i=0; i<data_size-1; i+=lod)
         {
            if(i+lod >= data_size) 
                i = data_size-lod;

            for(int j=0; j<data_size-1; j+=lod)
            {
               if(j+lod >= data_size) 
                  j = data_size-lod;
               data.push_back(_data[i*data_size + j]);
            }
         }
      }

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
   }

   void setBuffers(std::vector<int>& indices)
   {
      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
   }

   void setShader(Shader& shader)
   {
      shader.use();
      shader.setFloat("verticalAngle", verticalAngle);
      shader.setFloat("horizontalAngle", horizontalAngle);
      shader.setFloat("lod", lod);
   }

   void draw(std::vector<int>& indices)
   {
      glDrawElements(GL_TRIANGLE_STRIP, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
   }

   const std::vector<int>& getIndices()
   {
      return indices;
   }


private:
   std::vector<float> data;
   const float verticalAngle, horizontalAngle;

   unsigned int VBO, VAO, EBO;
   int lod;
   int dataRows, dataColumns;
   std::vector<int> indices;
};


struct Tiles
{
   Tiles(Shader& s): currIndicesIdx(3), shader(s)
   {
      for(auto lod : supportedLods)
         generateIndices(lod);
   }

   std::vector<Tile> tiles;
   std::vector<std::pair<int, std::vector<int>>> indicesVecs;
   int currIndicesIdx;
   Shader& shader;

   void updateCurrIndicesIdx(int lod)
   {
      for (int i=0; i<indicesVecs.size(); i++)
         if (indicesVecs[i].first == lod)
            currIndicesIdx = i;
   }

   void draw()
   {
      for(auto t : tiles)
      {
         std::vector<int>& ids = indicesVecs[currIndicesIdx].second;
         t.setBuffers(ids);
         t.setShader(shader);
         t.draw(ids);
      }
   }


   void generateIndices(int lod)
   {
      std::vector<int> indices;
      int size = MAX_TILE_SIZE/lod;
      for(int i=0; i<size-1; i+=1)
      {
         int k1 = i*size;
         int k2 = (i+1)*size;

         for (int j=0; j<size-1; j+=1)
         {
               indices.push_back(k1 + j+1);
               indices.push_back(k1 + j);
               indices.push_back(k2 + j);

               indices.push_back(k1 + j+1);
               indices.push_back(k2 + j+1);
               indices.push_back(k2 + j);
         }
      }
      indicesVecs.emplace_back(lod, indices); 
   }

};