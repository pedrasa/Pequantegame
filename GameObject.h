#pragma once

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif
#include <map>
#include <iostream>
#include "Collider.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//code framework taken from coventry university moodle 212

using namespace glm;


class GameObject
{
protected:

public:
	Collider* collider = NULL;

	static std::map <char, bool> keys;
	static std::map <int, bool> specialKeys;
	static bool debugMode;
	static float d, d2, d3, d4, d5, d6;
	bool active = true;
	mat4 modelViewMat = mat4(1.0);


	vec3 heading = vec3(0.0, 0.0, -1.0); // direction object faces
	vec3 position = vec3(0.0, 0.0, 0.0);

	GameObject(glm::vec3 position);
	~GameObject();

	virtual unsigned int setupDrawing(unsigned int listBase) = 0;
	virtual void drawScene();
	virtual void start() = 0;
	virtual void update(int deltaTime) = 0;
	virtual void collides(Collider* other);
};

