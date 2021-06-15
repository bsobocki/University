#include "Factorial.hpp"
#include <vector>
#include <iostream>

class Calculator
{
   public:
      Calculator();
      int add(int a, int b);
      int sub(int a, int b);
      int mult(int a, int b);
      int fact(int n);
      bool isPrime(int n);
      std::vector<int> primeDividers(int n);

   private:
      Factorial factorial;
};