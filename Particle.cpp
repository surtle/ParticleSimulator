#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle()
{
	position = glm::vec3(0);
	velocity = glm::vec3(0);
	acceleration = glm::vec3(0);

	mass = 0;
	life = -1;
	radius = 0;
	force = glm::vec3(0);
	isStatic = false;
}

Particle::Particle(glm::vec3 pos, float m, float r)
{
	position = pos;
	mass = m;
	radius = r;
	life = -1;

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

void Particle::computeAero(glm::vec3 v_air, float p, float c_d)
{
	// find relative velocity
	glm::vec3 v_rel = velocity - v_air;
	float v_rel_length = length(v_rel);

	// find e, unit vector opposite of velocity
	glm::vec3 e = -normalize(v_rel);

	// find normal
	glm::vec3 normal = normalize(velocity);

	// calculate area of particle
	float a_0 = 3.14 * radius * radius;
	float a = a_0 * ((dot(v_rel, normal)) / glm::length(v_rel));

	// calculate force
	glm::vec3 f_aero = (float)0.5 * p * v_rel_length * v_rel_length * c_d * a * e;

	applyForce(f_aero);
}

void Particle::computeAcceleration()
{
	glm::vec3 acc = (float)(1.0 / mass) * force;
	//cout << "  force: " << force.x << " " << force.y << " " << force.z << endl;
	acceleration = acc;
	//cout << "  acceleration: " << acc.x << " " << acc.y << " " << acc.z << endl;
}

void Particle::addNormal(glm::vec3 n)
{
	normal += n;
}

void Particle::setPos(glm::vec3 p)
{
	position = p;
}

void Particle::setVelocity(glm::vec3 v)
{
	velocity = v;
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

void Particle::resetLife(float l)
{
	life = l;
}

float Particle::getLife()
{
	return life;
}

void Particle::subLife(float delta)
{
	life -= delta;
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
