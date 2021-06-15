#include <gtest/gtest.h>
#include <Calculator.hpp>

class CalculatorTest : public ::testing::Test
{
   public:
      CalculatorTest(){}

   protected:
      Calculator calc;
};

TEST_F(CalculatorTest, add)
{
   EXPECT_EQ(0, calc.add(0,0));
   EXPECT_EQ(3, calc.add(1,2));
   EXPECT_EQ(88, calc.add(80, 8));
   EXPECT_EQ(10, calc.add(5, 5));
}

TEST_F(CalculatorTest, sub)
{
   EXPECT_EQ(2, calc.sub(3,1));
   EXPECT_EQ(0, calc.sub(100, 100));
}

TEST_F(CalculatorTest, mult)
{
   EXPECT_EQ(0, calc.mult(100, 0));
   EXPECT_EQ(100, calc.mult(10, 10));
}

/* zalozenie: n jest liczba naturalna */
TEST_F(CalculatorTest, fact)
{
   EXPECT_EQ(120, calc.fact(5));
   EXPECT_EQ(1, calc.fact(0));
   EXPECT_EQ(3628800, calc.fact(10));
}

/* zalozenie: n jest liczba naturalna */
TEST_F(CalculatorTest, isPrime)
{
   EXPECT_FALSE(calc.isPrime(0));
   EXPECT_TRUE(calc.isPrime(17));
   EXPECT_FALSE(calc.isPrime(12));
   EXPECT_TRUE(calc.isPrime(151));
   EXPECT_FALSE(calc.isPrime(290));
   EXPECT_FALSE(calc.isPrime(1));
   EXPECT_TRUE(calc.isPrime(2));
}

/* zalozenie: n jest liczba naturalna wieksza od zero */
TEST_F(CalculatorTest, primeDividers)
{
   std::vector<int> res27 = {3,3,3};
   EXPECT_EQ(res27, calc.primeDividers(27));

   std::vector<int> res48 = {2,2,2,2,3};
   EXPECT_EQ(res48, calc.primeDividers(48));

   std::vector<int> res23142 = {2,3,7,19,29};
   EXPECT_EQ(res23142, calc.primeDividers(23142));

   std::vector<int> res1 = {};
   EXPECT_EQ(res1, calc.primeDividers(1));
}