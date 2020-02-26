#include "Particle.h"
#pragma once

class Triangle
{
public:
	Triangle();
	Triangle(Particle* p1, Particle* p2, Particle* p3);
	~Triangle();

	glm::vec3 getNormal();
	void calculateNormal();

	Particle* getP(int p);
private:

	Particle* p1;
	Particle* p2;
	Particle* p3;

	glm::vec3 normal;
};

