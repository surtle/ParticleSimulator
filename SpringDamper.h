#pragma once
#include "Core.h"
#include "Particle.h"

class SpringDamper
{
public:
	SpringDamper();
	SpringDamper(Particle* p_1, Particle* p_2);
	~SpringDamper();

	void computeForce();
private:
	float springConstant = 70;
	float dampingFactor = 1.3;
	float restLength;

	Particle* p1;
	Particle* p2;
};

