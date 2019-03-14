#ifndef SPHERE_H
#define SPHERE_H

#include "vertex.h"
#include <vector>
#include "Collider.h"

using namespace glm;
using namespace std;

class collisionManager;

class Sphere
{
private:
	int sizeX;
	int sizeY;
	//Shere creation
	//REFERENCE: 'FieldAndSkyFilteredShaderized.sln'. From Computer Graphics Through OpenGL by Samanta Guha
	VertexWithNormal *sphereVerticesNor;  //121
	unsigned int *sphereIndices; //660

	int stacks; //10
	int slices; //10

	void CreateSpherewithNormal();
public:
	Collider* collider = NULL;

	int idd;
	//static int counter = 0;

	static enum object { FIELD, SKY, SPHERE, SKY2, SPHERE2, GRAVEL}; // VAO ids.
	static enum buffer { FIELD_VERTICES, SKY_VERTICES, SPHERE_VERTICES, SPHERE_INDICES, SKY2_VERTICES, GRAVEL_VERTICES}; // VBO ids.
	static int idChecker;

	vec3 positionC;
	vec3 directionC;
	
	float radius;

	Sphere();
	Sphere(vec3 pos,  int i);
	~Sphere();

	void draw();
	void update(int deltaTime);
	void updatePos();

	VertexWithNormal * GetVerData(int &);
	unsigned int * GetTriData(int &);
};


#endif