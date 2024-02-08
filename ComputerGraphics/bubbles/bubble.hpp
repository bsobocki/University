#ifndef BUBBLE_HPP
#define BUBBLE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "camera.hpp"

struct bubble_info
{
   glm::vec3 color;
   glm::vec3 position;
   float scale;
};

struct bubble_dist
{
   int bubble_index;
   float dist;

   bubble_dist(int i, float d): bubble_index(i), dist(d) {}
};

class BubblesOrderer
{
public:
   BubblesOrderer(Camera & c, std::vector<bubble_info>& b): cam(c), bubbles(b) {}

   std::vector<int> getBubblesOrder()
   {
      std::vector<bubble_dist> dists;
      std::vector<int> indices;

      for(int i=0; i<bubbles.size(); i++)
      {
         glm::vec3 distVec = bubbles[i].position - cam.getPosition();
         dists.emplace_back(i, glm::length(distVec));
      }

      std::sort(dists.begin(), dists.end(), [](const bubble_dist& a, const bubble_dist& b) { return a.dist > b.dist;});
      std::transform(dists.begin(), dists.end(), std::back_inserter(indices), [](const bubble_dist& a) -> int {return a.bubble_index;});

      return indices;
   }

private:
   Camera& cam;
   std::vector<bubble_info>& bubbles;
};

#endif