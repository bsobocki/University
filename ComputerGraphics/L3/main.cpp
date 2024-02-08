#include <Game.hpp>

int main(int argc, char *argv[]) {
   int N = 10;
   int maxRotate=2000;
   int seed = 5;
   int winSize = 1000;
   
   if (argc >= 2 and (seed = atoi(argv[1])) <= 0)
   {
      std::cout<<"Wrong seed value: '"<<argv[1]<<"'! please use positive integer..\n";
      return 1;
   }

   if (argc >= 3 and (N = atoi(argv[2])) <= 0)
   {
      std::cout<<"Wrong split value: '"<<argv[2]<<"'! please use positive integer..\n";
      return 1;
   }

   Game win;
   win.setSplit(N);
   win.setMaxRotate(maxRotate);
   win.Init(winSize,winSize,"AGL3 example",0,33);
   win.MainLoop(seed);
   return 0;
}
