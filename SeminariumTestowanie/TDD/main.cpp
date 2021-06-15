#include "src/Calculator.hpp"

int main()
{
   Calculator calc;
   for (auto x : calc.primeDividers(27))
      std::cout<<x<<std::endl;
}