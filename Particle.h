#pragma once
#include "Core.h"
class Particle
{
public:
	Particle();
	Particle(glm::vec3 pos, float m);
	~Particle();

	void computeForces(std::vector<glm::vec3> forces);
	void applyForce(glm::vec3 f);
	void computeAcceleration();
	void addNormal(glm::vec3 n);
	void normalizeNormal();
	void resetForces();
	void resetNormal();

	void moveX(float delta);
	void moveY(float delta);
	void moveZ(float delta);

	glm::vec3 getPos();
	glm::vec3 getVelocity();
	float getMass();
	void setStatic(bool s);
	bool getStatic();
	glm::vec3 getNormal();

	void update(float delta);

private:
	bool isStatic;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float mass;
	glm::vec3 force;

	
};

