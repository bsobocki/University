#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Cross.hpp>

// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================

MyCross::MyCross() : AGLDrawable(0) {
   setShaders();
   setBuffers();
}

void MyCross::setShaders() {
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

void MyCross::setBuffers() {
   bindBuffers();
   GLfloat vert[4][2] = {  // Cross lines
      { -1,  0  },
      {  1,  0  },
      {  0, -1  },
      {  0,  1  }
   };

   glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), vert, GL_STATIC_DRAW );
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

void MyCross::draw(float tx, float ty) {
   setCenter(tx,ty);
   bindProgram();
   bindBuffers();
   glUniform1f(0, scale);  // scale  in vertex shader
   glUniform2f(1, tx, ty);  // center in vertex shader
   glUniform3f(3, cross_color[0],cross_color[1],cross_color[2]);

   glDrawArrays(GL_LINES, 0, 4);
}

void MyCross::setColor(float r, float g, float b){
   cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
}

void MyCross::setCenter(float tx, float ty){
   centerX = tx;
   centerY = ty;
}

GLfloat MyCross::getCenterX() const{
   return centerX;
}

GLfloat MyCross::getCenterY() const{
   return centerY;
}