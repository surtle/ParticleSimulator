#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

void Vertex::setPosition(vec3 pos)
{
	position = pos;
}

void Vertex::setNormal(glm::vec3 norm)
{
	normal = norm;
}

void Vertex::addJoint(int j)
{
	joints.push_back(j);
}

void Vertex::addWeight(int index, float w)
{
	weights[index] = w;
}

vec3 Vertex::getPosition()
{
	return position;
}

glm::vec3 Vertex::getNormal()
{
	return normal;
}

std::vector<int> Vertex::getJoints()
{
	return joints;
}

std::map<int, float> Vertex::getWeights()
{
	return weights;
}
