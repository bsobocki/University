#pragma once
#include <AGL3Drawable.hpp>

class MyCross : public AGLDrawable {
public:
   MyCross();
   void setShaders();
   void setBuffers();
   void draw(float tx, float ty);
   void setColor(float r, float g, float b);
   void setCenter(float tx, float ty);

   GLfloat getCenterX() const;
   GLfloat getCenterY() const;

   const float scale = 0.0625;

private:
   GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
   GLfloat centerX;
   GLfloat centerY;
};