#include <vector>
#include <cmath>
#include <string>
#include <array>
#include <TriangleInfo.hpp>

struct point{
   float x,y;
   std::string toString() { return std::string("{") + std::to_string(x) + ", " + std::to_string(y) + "}"; };
};

struct line{
   float a,b;
   std::string toString() { return std::string("{a=") + std::to_string(a) + ", b=" + std::to_string(b) + "}"; };
};

using segment = std::pair<point, point>; 

bool isCollision(TriangleInfo t1, TriangleInfo t2);
std::vector<segment> getTriangleSegments(TriangleInfo t1);
bool isSegmentsCollision(segment s1, segment s2);
line getLineFromSegment(segment & s);
bool belongsToSegment(point p, segment s);
bool inXSegment(float x, segment s);
bool inYSegment(float y, segment s);