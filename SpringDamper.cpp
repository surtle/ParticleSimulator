#include "SpringDamper.h"

SpringDamper::SpringDamper()
{
}

SpringDamper::SpringDamper(Particle* p_1, Particle* p_2)
{
	p1 = p_1;
	p2 = p_2;

	restLength = glm::length(p2->getPos() - p1->getPos());
}

SpringDamper::~SpringDamper()
{
}

void SpringDamper::computeForce()
{
	// compute the distance l between the two points in the process
	float l = length(p2->getPos() - p1->getPos());
	glm::vec3 e = (p2->getPos() - p1->getPos())/l;

	// find 1D velocities
	float v1 = dot(e, p1->getVelocity());
	float v2 = dot(e, p2->getVelocity());

	// find 1D force
	float f_sd = -springConstant * (restLength - l) - dampingFactor * (v1 - v2);

	// map 1D force back into 3D
	glm::vec3 f1 = f_sd * e; 
	glm::vec3 f2 = -f1; 

	// apply forces
	p1->applyForce(f1);
	p2->applyForce(f2);
}
