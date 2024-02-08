#version 330 core
uniform int isLine;
out vec4 color;

void main()
{
   if (isLine == 1) 
   {
      color = vec4(0.68, 0.73, 0.9, 0.2);
   }
   else
   {
      color = vec4(1.0, 0.0, 0.0, 0.7);
   }
}