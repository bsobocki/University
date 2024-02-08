#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Circle.hpp>

#include <cmath>

// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================

MyCircle::MyCircle() : AGLDrawable(0) {
   r = 0.125;
   setShaders();
   setBuffers();
}

void MyCircle::setShaders() {
   compileShaders(R"END(

      #version 330 
      #extension GL_ARB_explicit_uniform_location : require
      #extension GL_ARB_shading_language_420pack : require
      layout(location = 0) in vec2 pos;
      layout(location = 0) uniform float scale;
      layout(location = 1) uniform vec2  center;
      out vec4 vtex;

      void main(void) {
         vec2 p = (pos * scale + center);
         gl_Position = vec4(p, 0.0, 1.0);
      }

   )END", R"END(

      #version 330 
      #extension GL_ARB_explicit_uniform_location : require
      layout(location = 3) uniform vec3  cross_color;
      out vec4 color;

      void main(void) {
         color = vec4(cross_color,1.0);
      } 

   )END");
}

void MyCircle::setCoords() {
   #define PI 3.1415926f
   #define NthPART float(i)/float(N)

   for (int i=0; i<N; i++) {
      float theta = 2.0f * PI * NthPART;
      vertex[i][0] = r * cosf(theta);
      vertex[i][1] = r * sinf(theta);
   }
}

void MyCircle::setCenter(float tx, float ty){
   centerX = tx;
   centerY = ty;
}

void MyCircle::setBuffers() {
   setCoords();
   bindBuffers();
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(
      0,                 // attribute 0, must match the layout in the shader.
      2,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,//24,             // stride
      (void*)0            // array buffer offset
   );
}

void MyCircle::draw(float tx, float ty) {
   setCenter(tx,ty);
   bindProgram();
   bindBuffers();
   glUniform1f(0, 1.0);
   glUniform2f(1, tx, ty);

   GLfloat red = tx;
   GLfloat green = ty;
   GLfloat blue = 1 - tx - ty;

   glUniform3f(3, red, green, blue);
   glBufferData(GL_ARRAY_BUFFER, N*2*sizeof(float), vertex, GL_STATIC_DRAW );
   glDrawArrays(GL_TRIANGLE_FAN, 0, N);
}

std::vector<std::vector<GLfloat>> MyCircle::getPoints() {
   std::vector<std::vector<GLfloat>> vec;
   for (int i=0; i<N; i++)
      vec.push_back({vertex[i][0] + centerX, vertex[i][1] + centerY});
   return vec;
}

float MyCircle::getCenterX() const{
   return centerX;
}

float MyCircle::getCenterY() const{
   return centerY;
}