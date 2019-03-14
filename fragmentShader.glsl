#version 430 core

#define FIELD 0
#define SKY 1
#define SPHERE 2
#define SKY2 3
#define SPHERE2 4

in vec2 texCoordsExport;
in vec3 normalExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;
  
struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material sphereFandB;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform sampler2D sphereTex;
uniform uint object;

out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor, sphereTexColor;
vec3 normal, lightDirection;
vec4 fAndBDif;


void main(void)
{  
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   sphereTexColor = texture(sphereTex, texCoordsExport);
   
   if (object == FIELD) colorsOut = fieldTexColor;
   //if (object == SKY) colorsOut = skyTexColor;
   if (object == SKY2) colorsOut = skyTexColor;
   if (object == SPHERE) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);    
   }
   if (object == SPHERE2) {
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);  
   }
}