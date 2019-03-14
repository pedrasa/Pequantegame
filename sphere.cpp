#include <fstream>
#include <iostream>

#include "vertex.h"
#include "sphere.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;

Sphere::Sphere()
{
	//counter = counter + 1;
	stacks = 10;
	slices = 10;
	radius = 1.0f;

	sphereVerticesNor = (VertexWithNormal *)malloc(sizeof(VertexWithNormal) * 121);
	sphereIndices = (unsigned int *)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
}

Sphere::Sphere(vec3 pos, int i)
{
	stacks = 10;
	slices = 10;
	radius = 1.0f;

	positionC = pos;
	directionC = vec3(0.0f, 0.0f, 0.0f);

	idd = i;

	sphereVerticesNor = (VertexWithNormal *)malloc(sizeof(VertexWithNormal) * 121);
	sphereIndices = (unsigned int *)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();

}

Sphere::~Sphere()
{
	free(sphereVerticesNor);
	free(sphereIndices);
	delete collider;
	collider = NULL;
}

void Sphere::CreateSpherewithNormal(void)
{
	int count;
	count = 0;
	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;
			GLfloat theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = vec4(x * radius, y * radius + 6.0, z * radius, 1.0);
			sphereVerticesNor[count].normals = vec3(x, y, z); ///Sphere normals
			count++;
		}
	}

	count = 0;
	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;


}


void Sphere::updatePos()
{
}

VertexWithNormal * Sphere::GetVerData(int &verNum)
{
	verNum = 121;
	return sphereVerticesNor;
}

unsigned int * Sphere::GetTriData(int &triNum)
{
	triNum = 660;
	return sphereIndices;
}

void Sphere::draw()
{
	//Currently not working.
	this->collider->Draw();
}

void Sphere::update(int deltaTime)
{
}


