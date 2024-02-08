#version 330 core

uniform mat4 view;
uniform mat4 projection;

uniform float verticalAngle;
uniform float horizontalAngle;
uniform float lod;

layout (location = 0) in float terrain_height;
out float ht;

const float radius = 6373.0f;
const int DEFAULT_SIZE = 1201;
const float correctRotation = 180.f;

void main()
{
   int size = DEFAULT_SIZE/int(lod);
   float stepAngle = 1.0/float(size-1);

   ht = terrain_height;
   if (ht > 9000) ht = 0.f;

   int row = int(gl_VertexID) / size;
   int column = int(gl_VertexID) % size;

   float verAngle = (verticalAngle + 1.f) - row*stepAngle;
   float horAngle = horizontalAngle + column*stepAngle + correctRotation;

   float x = cos(radians(verAngle)) * sin(radians(horAngle)) * radius;
   float y = sin(radians(verAngle)) * radius;
   float z = cos(radians(verAngle)) * cos(radians(horAngle)) * radius;

   // jesli chcemy wypukle to zakomentowac wyzsze i odkomentowac to
   // float x = cos(radians(verAngle)) * sin(radians(horAngle)) * (radius + ht/1000);
   // float y = sin(radians(verAngle)) * (radius + ht/1000); 
   // float z = cos(radians(verAngle)) * cos(radians(horAngle)) * (radius + ht/1000); 
   
   gl_Position = projection * view * vec4(x,y,z,1.0);
}
