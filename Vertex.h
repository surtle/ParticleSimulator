#pragma once
#include <vector>
#include "Core.h"
#include "glm/detail/type_vec.hpp"
#include <map>

using namespace glm;

class Vertex
{
public:
	Vertex();
	~Vertex();

	void setPosition(glm::vec3 pos);
	void setNormal(glm::vec3 norm);
	void addJoint(int j);
	void addWeight(int index, float w);

	glm::vec3 getPosition();
	glm::vec3 getNormal();
	std::vector<int> getJoints();
	std::map<int, float> getWeights();

private:
	glm::vec3 position;
	glm::vec3 normal;

	std::vector<int> joints; 
	std::map<int, float> weights;
};

