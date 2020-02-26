#pragma once
#include "Model.h"
#include "Core.h"

class Floor
{
public:
	Floor();
	void draw(glm::mat4 viewMatr, uint shader);

private:
	Model* floor;

	glm::vec3 p1 = glm::vec3(-10, -2, -10);
	glm::vec3 p2 = glm::vec3(10, -2, -10);
	glm::vec3 p3 = glm::vec3(-10, -2, 10);
	glm::vec3 p4 = glm::vec3(10, -2, 10);

	glm::vec3 n = glm::vec3(0, 1, 0);

	std::vector<ModelVertex> vertices =
	{
		{p1, n},
		{p2, n},
		{p3, n},

		{p2, n},
		{p4, n},
		{p3, n}
	};

	std::vector<uint> indices = { 0, 1, 2, 3, 4, 5 };
};

