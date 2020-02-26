#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle()
{
	position = glm::vec3(0);
	velocity = glm::vec3(0);
	acceleration = glm::vec3(0);

	mass = 0;
	force = glm::vec3(0);
	isStatic = false;
}

Particle::Particle(glm::vec3 pos, float m)
{
	position = pos;
	mass = m;

	isStatic = false;
	force = glm::vec3(0);
	velocity = glm::vec3(0);
	acceleration = glm::vec3(0);
}

Particle::~Particle()
{
}

void Particle::computeForces(std::vector<glm::vec3> forces)
{
	// sum all forces
	for (glm::vec3 f : forces) {
		applyForce(f);
	}
}

void Particle::applyForce(glm::vec3 f)
{
	if (isStatic) return;
	force += f;
}

void Particle::computeAcceleration()
{
	glm::vec3 acc = (float)(1.0 / mass) * force;
	acceleration = acc;
}

void Particle::addNormal(glm::vec3 n)
{
	normal += n;
}

void Particle::normalizeNormal()
{
	normal = normalize(normal);
}

void Particle::resetForces()
{
	force = glm::vec3(0);
}

void Particle::resetNormal()
{
	normal = glm::vec3(0);
}

void Particle::moveX(float delta)
{
	position += glm::vec3(delta, 0, 0);
}

void Particle::moveY(float delta)
{
	position += glm::vec3(0, delta, 0);
}

void Particle::moveZ(float delta)
{
	position += glm::vec3(0, 0, delta);
}

glm::vec3 Particle::getPos()
{
	return position;
}

glm::vec3 Particle::getVelocity()
{
	return velocity;
}

float Particle::getMass()
{
	return mass;
}

void Particle::update(float delta)
{
	if (!isStatic) {
		glm::vec3 nextV, nextR;

		// compute values for v_(t+1) and r_(t+1)
		nextV = velocity + acceleration * delta;
		nextR = position + nextV * delta;

		// set new values for velocity and position 
		velocity = nextV;
		position = nextR;
	}
}

void Particle::setStatic(bool s)
{
	isStatic = s;
}

bool Particle::getStatic()
{
	return isStatic;
}

glm::vec3 Particle::getNormal()
{
	return normal;
}
