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
	// loop through particles and zero out normal
	normal = normalize(cross(p3->getPos() - p1->getPos(), p2->getPos() - p1->getPos()));

	p1->addNormal(normal);
	p2->addNormal(normal);
	p3->addNormal(normal);
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
