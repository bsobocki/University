#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Background.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
Background::Background() : AGLDrawable(0) {
   setShaders();
}

void Background::setShaders() {
   compileShaders(R"END(
      #version 330 
      #extension GL_ARB_explicit_uniform_location : enable
      layout(location = 1) uniform float scale;
      out vec4 vcolor;
      out vec2 position;
      void main(void) {
         const vec2 vertices[] = vec2[4]( vec2(-0.95, -0.95),
                                          vec2( 0.95, -0.95),
                                          vec2(-0.95,  0.95),
                                          vec2( 0.95,  0.95));
         const vec4 colors[]    = vec4[4](vec4(1.0, 0.0, 0.0, 1.0),
                                          vec4(0.0, 1.0, 0.0, 1.0),
                                          vec4(0.0, 0.0, 1.0, 1.0),
                                          vec4(0.0, 1.0, 1.0, 1.0));

         vcolor      = colors[gl_VertexID];
         gl_Position = vec4(vertices[gl_VertexID]* scale, 0.5, 1.0) ; 
         position = gl_Position.xy;
      }

   )END", 
   
   R"END(
      #version 330
      #extension GL_ARB_separate_shader_objects : enable
      #extension GL_ARB_explicit_uniform_location : enable
      layout(location = 0) uniform float timeElapsed;
      in  vec4 vcolor;
      out vec4 color;
      in vec2 position;
      out float average;
      out float area;
      out float light;

      void main(void) {
         // odkomentowując każdy z kolei mamy różne typy tła
         // lubię trochę eksperymantować, stąd powstało parę ciekawych moim zdaniem efektów :)
         //color = vec4(sin(position.x*30)-0.5, cos(position.y*29)-0.5, (position.x + position.y)*0.5, vcolor[3]);
         //color = vec4(sin(position.x*30) + sin(position.y*30) - 1.0, cos(position.x*30) + cos(position.y*29) - 1.0, 0.0, vcolor[3]);
         //color = vec4(sin(position.x*30*timeElapsed) + sin(position.y*30*timeElapsed) - 0.2, cos(position.x*30) + cos(position.y*30) + timeElapsed - 0.2, 0.0, vcolor[3]);
         //color = vec4(abs(position.x + position.y)/2.0*timeElapsed + 0.4, abs(position.x - position.y)*timeElapsed - 0.2, abs(position.x + position.y)*timeElapsed, vcolor[3]);
         //color = vec4(sin(position.x*30)*timeElapsed, cos(position.y*29)*timeElapsed, (position.x + position.y)*timeElapsed, vcolor[3]);
         color = vec4(0.3, sin(position.x + 30*timeElapsed) - sin(position.y + 30*timeElapsed), cos(position.x*30) + cos(position.y*30) + timeElapsed - 0.2, vcolor[3]);
      } 

   )END");
}

void Background::draw(float scale, float time) {
   bindProgram();
   //bindBuffers();
   glUniform1f(0, time);
   glUniform1f(1, scale);  // scale  in vertex shader
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
