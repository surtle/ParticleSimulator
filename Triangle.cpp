#include "Triangle.h"

Triangle::Triangle(Particle* part1, Particle* part2, Particle* part3)
{
	p1 = part1;
	p2 = part2;
	p3 = part3;
}

glm::vec3 Triangle::getNormal()
{
	return normal;
}

void Triangle::calculateNormal()
{
	normal = normalize(cross(p2->getPos() - p1->getPos(), p3->getPos() - p1->getPos()));

	// add normal to each intersecting point
	p1->addNormal(normal);
	p2->addNormal(normal);
	p3->addNormal(normal);
}

void Triangle::applyAeroForce(glm::vec3 v_air, float p, float c_d)
{
	// find average velocity of triangle
	glm::vec3 v_surface = (p1->getVelocity() + p2->getVelocity() + p3->getVelocity()) / (float) 3.0;

	// find relative velocity
	glm::vec3 v_rel = v_surface - v_air;
	float v_rel_length = length(v_rel);

	// find e, unit vector opposite of velocity
	glm::vec3 e = -normalize(v_rel);

	// find triangle normal
	normal = normalize(cross(p2->getPos() - p1->getPos(), p3->getPos() - p1->getPos()));

	// calculate area of triangle exposed to air flow
	float a_0 = 0.5 * glm::length(cross(p2->getPos() - p1->getPos(), p3->getPos() - p1->getPos()));
	float a = a_0 * ((dot(v_rel, normal)) / glm::length(v_rel));

	// calculate force
	glm::vec3 f_aero = (float)0.5 * p * v_rel_length * v_rel_length * c_d * a * e;
	
	// apply average of aerodynamic force onto three particles 
	p1->applyForce(f_aero / (float)3.0);
	p2->applyForce(f_aero / (float)3.0);
	p3->applyForce(f_aero / (float)3.0);
	
}

Particle* Triangle::getP(int p)
{
	switch (p) {
	case 1:
		return p1;
	case 2:
		return p2;
	case 3:
		return p3;
	}
}
