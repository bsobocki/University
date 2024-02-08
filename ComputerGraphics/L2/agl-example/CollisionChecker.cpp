#include <CollisionChecker.hpp>

bool CollisionChecker::isCollisionWithCrossHorizontalLine(MyCross & cross, point P1, point P2)
{
   // check the point of intersection (point I) of the segment [P1, P2] and the horizontal cross segment
   // if I.x is between X coordinates of the cross horizontal segment endpoints and crossY is between P1.y and P2.y then there is a collision
   // y = ax + b
   // a = (P1.y - P2.y)/(P1.x - P2.x)
   // b = y - ax   =>    b = P1.y - aP1.x
   // x = (crossY - b) / a
   GLfloat crossX = cross.getCenterX();
   GLfloat crossY = cross.getCenterY();
   GLfloat crossLeftX  = crossX - cross.scale;
   GLfloat crossRightX = crossX + cross.scale;

   GLfloat a = (P1.y - P2.y)/(P1.x - P2.x);
   GLfloat b = P1.y - a*P1.x;

   if (P1.y == P2.y){
      if (P1.y == crossY){
         if((crossLeftX <= P1.x and P1.x <= crossRightX) or
            (crossLeftX <= P2.x and P2.x <= crossRightX))
            return true;
      }
      return false;
   }

   GLfloat x = (crossY - b)/a;

   return (crossLeftX <= x and x <= crossRightX) and
          (std::min(P1.y, P2.y) <= crossY and crossY <= std::max(P1.y, P2.y));
}

bool CollisionChecker::isCollisionWithCrossVerticalLine(MyCross & cross, point P1, point P2)
{
   // check the point of intersection (point I) of the segment [P1, P2] and the vertical cross segment
   // if I.y is between Y coordinates of the cross horizontal segment endpoints and crossX is between P1.x and P2.x then there is a collision
   // y = ax + b
   // a = (P1.y - P2.y)/(P1.x - P2.x)
   // b = y - ax   =>    b = P1.y - aP1.x
   // y = a*crossX + b
   GLfloat crossX = cross.getCenterX();
   GLfloat crossY = cross.getCenterY();
   GLfloat crossDownY  = crossY - cross.scale;
   GLfloat crossUpY    = crossY + cross.scale;

   GLfloat a = (P1.y - P2.y)/(P1.x - P2.x);
   GLfloat b = P1.y - a*P1.x;

   GLfloat y = a*crossX + b;

   return (crossDownY <= y and y <= crossUpY) and
          (std::min(P1.x, P2.x) <= crossX and crossX <= std::max(P1.x, P2.x));
}

bool CollisionChecker::isCollision(MyCross & cross, MyCircle & circle){
   auto circlePoints = circle.getPoints();

   for(int i=0; i<circlePoints.size()-1; i++){
      if (isCollisionWithCrossHorizontalLine(cross, circlePoints[i], circlePoints[i+1]))
         return true;
      if (isCollisionWithCrossVerticalLine(cross, circlePoints[i], circlePoints[i+1]))
         return true;
   }
   
   return false;
}