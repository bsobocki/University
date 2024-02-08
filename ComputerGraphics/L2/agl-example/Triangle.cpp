#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Triangle.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
MyTri::MyTri() : AGLDrawable(0) {
   setShaders();
}

void MyTri::setShaders() {
   compileShaders(R"END(

      #version 330 
      out vec4 vcolor;
      out vec2 position;
      void main(void) {
         const vec2 vertices[3] = vec2[3](vec2( 0.9, -0.9),
                                          vec2(-0.9, -0.9),
                                          vec2( 0.9,  0.9));
         const vec4 colors[]    = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                          vec4(0.0, 1.0, 0.0, 1.0),
                                          vec4(0.0, 0.0, 1.0, 1.0));

         vcolor      = colors[gl_VertexID];
         gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         position = gl_Position.xy;
      }

   )END", R"END(

      #version 330 
      in  vec4 vcolor;
      out vec4 color;
      in vec2 position;
      out float average;
      out float area;
      out float light;

      void main(void) {
         area=0.1;
         light=1.0;
         if(position.x > -area && position.x < area && position.y > -area && position.y < area){
            average = abs(position.x/2) + abs(position.y/2);
            light = light-average;
            color=vec4(vcolor[0]+light, vcolor[1]+light, vcolor[2]+light, 1);
         }
         else
            color = vcolor;
      } 

   )END");
}

void MyTri::draw() {
   bindProgram();
   glDrawArrays(GL_TRIANGLES, 0, 3);
}