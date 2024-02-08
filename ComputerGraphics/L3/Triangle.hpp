#pragma once
#include <AGL3Drawable.hpp>
#include <array>
#include <TriangleInfo.hpp>

#define IS_PLAYER 1

class Triangle : public AGLDrawable {
public:
   Triangle(int _isPlayer = 0);
   void setShaders();
   void setBuffers();
   void draw(float scale, float tx, float ty);
   void setColor(float r, float g, float b);
   void setCenter(float tx, float ty);
   void setRotate(int _rotate);
   void setCoords();
   void setScale(float _scale);
   void setMaxRotate(int _maxRotate);

   GLfloat getCenterX() const;
   GLfloat getCenterY() const;
   float getScale()   { return scale; }
   int getRotate()    { return rotate; }
   int getMaxRotate() { return maxRotate; }
   TriangleInfo getTriangleInfo();

private:
   GLfloat centerX;
   GLfloat centerY;

   GLfloat tri_color[3][4] = {
      {0.0, 0.0, 0.1, 1.0},
      {0.0, 0.5, 0.8, 1.0},
      {1.0, 1.0, 0.3, 1.0}
   };

   int rotate = 0;
   int maxRotate = 2000;
   float scale = 0.0625;
   GLfloat vert[3][2];
   
   const float height = 1.0f;
   const float distCenterYTopY = 0.7f;
   const float width  = 0.5f;
   int isPlayer = false;
};