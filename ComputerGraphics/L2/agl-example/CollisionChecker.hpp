#include <Circle.hpp>
#include <Cross.hpp>

struct point { 
   point()=default;
   point(std::vector<GLfloat> v): x(v[0]), y(v[1]){}
   float x, y; 
};

class CollisionChecker{
public:
   bool isCollision(MyCross & cross, MyCircle & circle);

private:
   bool isCollisionWithCrossHorizontalLine(MyCross & cross, point P1, point P2);
   bool isCollisionWithCrossVerticalLine(MyCross & cross, point P1, point P2);
   const float epsilon = 0.0003;
};