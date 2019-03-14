#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec4 coords;
	glm::vec2 texCoords;
};

struct VertexWithNormal
{
	glm::vec4 coords;
	glm::vec3 normals;
};

#endif
