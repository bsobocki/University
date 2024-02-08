#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Triangle.hpp>

#include <cmath>

Triangle::Triangle(int _isPlayer) : AGLDrawable(0) {
   isPlayer = _isPlayer;
   setShaders();
   setBuffers();
}

void Triangle::setShaders() {
   compileShaders(R"END(
      #version 330 
      #extension GL_ARB_explicit_uniform_location : require
      #extension GL_ARB_shading_language_420pack : require
      layout(location = 0) in vec2 pos;
      layout(location = 0) uniform float scale;
      layout(location = 1) uniform vec2  center;
      layout(location = 2) uniform int isPlayer;
      layout(location = 3) uniform int rotate;
      layout(location = 4) uniform int maxRotate;
      out vec4 vtex;
      out vec4 colorVal;
      out vec2 position;
      vec2 vertices[3];
      float theta;
      const float PI_2 = 6.2831852;
      const float TOP = 0.0;
      const float RIGHT_BASE = 0.45;
      const float LEFT_BASE = 0.55;

      const vec4 regularColors[] = vec4[3](vec4(1.0, 0.0, 1.0, 0.0),
                                           vec4(1.0, 0.5, 0.0, 0.0),
                                           vec4(1.0, 0.5, 0.5, 0.0));

      const vec4 playerColors[] =  vec4[3](vec4(0.76, 0.41, 0.18, 1.0),
                                           vec4(0.91, 0.97, 0.65, 1.0),
                                           vec4(0.95, 0.35, 0.35, 1.0));

      void main(void) {
         theta = PI_2 * (TOP + float(rotate)/float(maxRotate));
         vertices[0] = vec2(cos(theta), sin(theta)) * scale + center;

         theta = PI_2 * (RIGHT_BASE + float(rotate)/float(maxRotate));
         vertices[1] = vec2(cos(theta), sin(theta)) * scale + center;

         theta = PI_2 * (LEFT_BASE + float(rotate)/float(maxRotate));
         vertices[2] = vec2(cos(theta), sin(theta)) * scale + center;

         gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0);

         position = gl_Position.xy;
         if (isPlayer == 1)
         {
            colorVal = playerColors[gl_VertexID];
         }
         else
         {
            colorVal = regularColors[gl_VertexID];
         }
      }

   )END", R"END(

      #version 330 
      #extension GL_ARB_explicit_uniform_location : require
      in vec4 colorVal;
      in vec2 position;
      out vec4 color;

      void main(void) {
         color = colorVal;
      } 

   )END");
}

void Triangle::setBuffers() {
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

void Triangle::draw(float scale, float tx, float ty) {
   setCenter(tx,ty);
   setScale(scale);
   bindProgram();
   bindBuffers();
   glUniform1f(0, scale);  // scale  in vertex shader
   glUniform2f(1, tx, ty);  // center in vertex shader
   glUniform1i(2, isPlayer);
   glUniform1i(3, rotate);
   glUniform1i(4, maxRotate);

   glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::setCenter(float tx, float ty)
{
   centerX = tx;
   centerY = ty;
}

void Triangle::setRotate(int _rotate)
{
   rotate=_rotate;
}

void Triangle::setMaxRotate(int _maxRotate)
{
   maxRotate = _maxRotate;
}

GLfloat Triangle::getCenterX() const{
   return centerX;
}

GLfloat Triangle::getCenterY() const{
   return centerY;
}

void Triangle::setScale(float _scale)
{
   scale=_scale;
}

TriangleInfo Triangle::getTriangleInfo()
{
   return {
      centerX,
      centerY,
      scale,
      rotate,
      maxRotate
   };
}