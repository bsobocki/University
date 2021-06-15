#include "Factorial.hpp"

Factorial::Factorial()
{
}

int Factorial::doFact(int n)
{
   int fact = 1;
   for (int i=2; i<=n; i++)
      fact *= i;
   return fact;


   /* podczas prezentacji uzywalismy rowniez kodu:

   if (n==0) return 1;
   return n * doFact(n-1);

   */
}