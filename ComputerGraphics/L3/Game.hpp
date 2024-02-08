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
#include <Triangle.hpp>
#include <Collision.hpp>
#include <Background.hpp>
#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <iterator>

namespace
{
   using Triangles = std::vector<Triangle>;

   // funkcja zwraca środek komórki
   // poprzez komórkę rozumiem po prostu pole takie, żeby podzielić całośc na N fragmentów
   point getCellCenter(int column, int row, int split)
   {
      // zakres, w jakim jest centerX i centerY to od -1.0 do -1.0, czyli długości 2.0
      // dzieląc go na split mamy ile wynosi jedna część
      // odejmując/dodając center podzielony na 2 mamy środek
      float center = 2.0f/float(split);

      return {-1.0f + float(column)*center + center/2,
               1.0f - float(row)*center - center/2};
   }

   void printFile(const char * path)
   {
      std::ifstream file(path);
      if (file.is_open())
        std::cout << file.rdbuf();
   }

}

class Game : public AGLWindow {
public:
    Game() {};
    Game(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop(int seed);
    void setSplit(int N);
    void setMaxRotate(int _maxRotate);

private:
   int split=20;
   int maxRotate=2000;
};


void Game::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
}

void Game::setSplit(int N)
{
   split=N;
}

void Game::setMaxRotate(int _maxRotate)
{
   maxRotate = _maxRotate;
}

void Game::MainLoop(int seed) {
   #define IsObstacle(row,col) row!=playerRowIndex or column!=playerColumnIndex
   srand(seed);
   ViewportOne(0,0,wd,ht);

   Background bckgrd;
   Triangles obstacle;
   Triangle player(IS_PLAYER);
   player.setMaxRotate(maxRotate);


   // dobrane metodą prób i błędów
   float scale = 1.0f/float(1.5f*split);
   int playerColumnIndex = 0;
   int playerRowIndex = split-1;
   int metaIndex = split-1;

   for (int row=0; row<split; row++)
   {
      for(int column=0; column<split; column++)
      {
         if ( IsObstacle(row,col) )
         {
            point center = getCellCenter(column, row, split);
            Triangle tr;
            tr.setRotate(rand()%maxRotate);
            tr.setMaxRotate(maxRotate);
            tr.setCenter(center.x, center.y);
            tr.setScale(scale);
            obstacle.push_back(tr);
         }
      }
   }

   point playerCenter = getCellCenter(playerColumnIndex, playerRowIndex, split);
   float playerX=playerCenter.x;
   float playerY=playerCenter.y;
   int rotate = 0;

   bool winning = false;
   bool endGame = false;

   float gameScale = 1.0f;
   float scaleReduction = 0.0007f;

   double backgroudAnimationTime = 0.0f;
   double delay = 0.400;

   #define MOVE 0.0005
   do {
      glfwSetTime(backgroudAnimationTime);
      scale *= gameScale;

      glClear( GL_COLOR_BUFFER_BIT );
   
      bckgrd.draw(gameScale, (float)backgroudAnimationTime);
      player.setRotate(rotate);
      player.draw(scale, playerX * gameScale, playerY * gameScale);
      
      for(auto obs : obstacle)
      {
         float scale = obs.getScale() * gameScale;
         float centerX = obs.getCenterX() * gameScale;
         float centerY = obs.getCenterY() * gameScale;
         obs.draw(scale, centerX, centerY);
      }

      WaitForFixedFPS();  //  <------ użycie funkcji do szybkości poruszania
      glfwSwapBuffers(win());
      glfwPollEvents(); 

      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS)
         playerY -= MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS)
         playerY += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS)
         playerX += MOVE;
      else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) 
         playerX -= MOVE;

      else if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS)
      {
         rotate++;
         if (rotate>=maxRotate) rotate-=maxRotate;
      }
      else if (glfwGetKey(win(), GLFW_KEY_D ) == GLFW_PRESS)
      {
         rotate--;
         if (rotate<0) rotate+=maxRotate;
      }

      for (unsigned int i = 0; i<obstacle.size() ; i++)
         if (isCollision(player.getTriangleInfo(), obstacle[i].getTriangleInfo()))
         {
            endGame = true;
            if (i == metaIndex) winning = true;
            break;
         }

      backgroudAnimationTime += 0.0004f;

      if (backgroudAnimationTime > 3.0)
         backgroudAnimationTime -= 3.0f;

      if (endGame) gameScale -= scaleReduction;

      if (glfwGetKey(win(), GLFW_KEY_ESCAPE ) == GLFW_PRESS)
         endGame = true;

   } while(gameScale > 0 and not glfwWindowShouldClose(win()));

   if (winning)
      ::printFile("win.txt");
   else
      ::printFile("lose.txt");
}