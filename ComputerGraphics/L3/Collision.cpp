#include <Collision.hpp>

bool isCollision(TriangleInfo t1, TriangleInfo t2)
{
   int collisionSide=0;

   auto t1Segments = getTriangleSegments(t1);
   auto t2Segments = getTriangleSegments(t2);

   for(auto seg1 : t1Segments)
      for (auto seg2 : t2Segments)
         if (isSegmentsCollision(seg1, seg2))
            return true;

   return false;
}

std::vector<segment> getTriangleSegments(TriangleInfo t1)
{
   #define PI_2 6.2831852f
   #define TOP 0.0f
   #define RIGHT_BASE 0.45f
   #define LEFT_BASE 0.55f

   std::array<point, 3> points;

   float theta = PI_2 * (TOP + float(t1.rotate)/float(t1.maxRotate));
   points[0] = {cosf(theta) * t1.scale + t1.centerX, 
                  sinf(theta) * t1.scale + t1.centerY};

   theta = PI_2 * (RIGHT_BASE + float(t1.rotate)/float(t1.maxRotate));
   points[1] = {cosf(theta) * t1.scale + t1.centerX, 
                  sinf(theta) * t1.scale + t1.centerY};

   theta = PI_2 * (LEFT_BASE + float(t1.rotate)/float(t1.maxRotate));
   points[2] = {cosf(theta) * t1.scale + t1.centerX, 
                  sinf(theta) * t1.scale + t1.centerY};

   return { {points[0], points[1]},
            {points[1], points[2]},
            {points[2], points[0]} };
}

bool isSegmentsCollision(segment s1, segment s2)
{
   float epsilon = 0.000001f;
   auto l1 = getLineFromSegment(s1);
   auto l2 = getLineFromSegment(s2);
   
   if (abs(l1.a - l2.a) <= epsilon)
      return false;

   float x = (l2.b - l1.b) / (l1.a - l2.a);
   float y = l1.a * x + l1.b;

   point p = {x,y};

   return belongsToSegment(p, s1) and belongsToSegment(p, s2);
}

line getLineFromSegment(segment & s)
{
   line l;
   l.a = (s.first.y - s.second.y) / (s.first.x - s.second.x);
   l.b = s.first.y - l.a*s.first.x;
   return l;
}

bool belongsToSegment(point p, segment s)
{
   return inXSegment(p.x, s) and inYSegment(p.y, s);
}

bool inXSegment(float x, segment s)
{
   return std::min(s.first.x, s.second.x) <= x and x <= std::max(s.first.x, s.second.x);
}

bool inYSegment(float y, segment s)
{
   return std::min(s.first.y, s.second.y) <= y and y <= std::max(s.first.y, s.second.y);
}
