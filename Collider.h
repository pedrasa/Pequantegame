#pragma once


#include <glm/glm.hpp>
class Collider
{
public:
	
	virtual bool collidesWith(Collider* other) = 0;
	glm::vec3* colliderCentre = NULL;

	virtual float minX() = 0;
	virtual float maxX() = 0;
	virtual float minY() = 0;
	virtual float maxY() = 0;
	virtual float minZ() = 0;
	virtual float maxZ() = 0;

	virtual float Radius() = 0;
	virtual void Draw() = 0;
};

