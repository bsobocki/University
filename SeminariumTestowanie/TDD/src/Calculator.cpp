#include "Calculator.hpp"
#include <cmath>

Calculator::Calculator()
{
}

int Calculator::add(int a, int b)
{
   return a + b;
   
   /* podczas prezentacji uzywalismy rowniez kodu (kod tylko dla a,b >= 0):

   while(b--)  a++;
   return a;

   */
}

int Calculator::sub(int a, int b)
{
   return a - b;

   /* podczas prezentacji uzywalismy rowniez kodu (kod tylko dla a,b >= 0):

   while(b--)  a--;
   return a;

   */
}

int Calculator::mult(int a, int b)
{
   return a * b;

   /* podczas prezentacji uzywalismy rowniez kodu (kod tylko dla a,b >= 0):

   int x = 0;
   while(b--)  x = add(x, a);
   return x;

   */
}

int Calculator::fact(int n)
{
   return factorial.doFact(n);
}

bool Calculator::isPrime(int n)
{
   if (n<2) return false;

   for (int i=2; i < sqrt(n); i++)
      if (n%i == 0)
         return false;

   return true;
}

std::vector<int> Calculator::primeDividers(int n)
{
   std::vector<int> divs;
   int i=2;

   while(n>1)
   {
      while(n%i==0 and isPrime(i))
      {
         divs.push_back(i);
         n/=i;
      }
      i++;
   }

   return divs;
}