#version 430 core

#define FIELD 0
#define SKY 1
#define SPHERE 2
#define SKY2 3
#define SPHERE2 4


layout(location=0) in vec4 fieldCoords;
layout(location=1) in vec2 fieldTexCoords;
layout(location=2) in vec4 skyCoords;
layout(location=3) in vec2 skyTexCoords;
layout(location=4) in vec4 sphereCoords;
layout(location=5) in vec3 sphereNormals;


uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

out vec2 texCoordsExport;
out vec3 normalExport;

vec4 coords;

void main(void)
{   
   if (object == FIELD)
   {
      coords = fieldCoords;
      texCoordsExport = fieldTexCoords;
   }
   /*if (object == SKY)
   {
      coords = skyCoords;
      texCoordsExport = skyTexCoords;
   }*/
   if (object == SKY2)
   {
      coords = skyCoords;
      texCoordsExport = skyTexCoords;
   }
   if (object == SPHERE)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
   if (object == SPHERE2)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
   
   gl_Position = projMat * modelViewMat * coords;
}