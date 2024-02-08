// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>
#include <Cross.hpp>
#include <Circle.hpp>
#include <Triangle.hpp>
#include <CollisionChecker.hpp>

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop();
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
       ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
       ; // do something
    }
}


// ==========================================================================
void MyWin::MainLoop() {
   ViewportOne(0,0,wd,ht);

   CollisionChecker collisions;

   MyCross  cross;
   MyTri    trian;
   MyCircle circle;

   #define MOVE 0.00025
   float crX=0.2, crY=0.2;
   float ciX=-0.2, ciY=-0.2;
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      trian.draw();
      cross.draw(crX,crY);
      circle.draw(ciX,ciY);
      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win());
      glfwPollEvents();
      //glfwWaitEvents();   

      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS)
         crY -= MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS)
         crY += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS)
         crX += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) 
         crX -= MOVE;

      if (glfwGetKey(win(), GLFW_KEY_SPACE ) == GLFW_PRESS)
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      if (glfwGetKey(win(), GLFW_KEY_ENTER ) == GLFW_PRESS)
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      if (glfwGetKey(win(), GLFW_KEY_S ) == GLFW_PRESS)
         ciY -= MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_W ) == GLFW_PRESS)
         ciY += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS)
         ciX += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS)
         ciX -= MOVE;

   } while( 
         not collisions.isCollision(cross, circle)          and
         glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS  and
         glfwWindowShouldClose(win()) == 0 
      );
}

int main(int argc, char *argv[]) {
   std::cout<<"Zachęcam do przeczytania README :)"<<std::endl;
   
   MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}
