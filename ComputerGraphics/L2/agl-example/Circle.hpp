#pragma once
#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <vector>

class MyCircle : public AGLDrawable {
public:
   static const int N = 100;

   MyCircle();
   void setShaders();
   void setBuffers();
   void draw(float tx, float ty);
   void setColor(float r, float g, float b);
   void setCoords();
   void setCenter(float tx, float ty);

   GLfloat getCenterX() const;
   GLfloat getCenterY() const;
   std::vector<std::vector<GLfloat>> getPoints();

private:
   float centerX;
   float centerY;
   float vertex [N][2];
   float r;
};