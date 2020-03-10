#pragma once
#include "Core.h"
class Particle
{
public:
	Particle();
	Particle(glm::vec3 pos, float m, float r);
	~Particle();

	void computeForces(std::vector<glm::vec3> forces);
	void applyForce(glm::vec3 f);
	void computeAero(glm::vec3 v_air, float p, float c_d);
	void computeAcceleration();
	void addNormal(glm::vec3 n);
	void setPos(glm::vec3 p);
	void setVelocity(glm::vec3 v);
	void normalizeNormal();
	void resetForces();
	void resetNormal();

	void moveX(float delta);
	void moveY(float delta);
	void moveZ(float delta);

	void resetLife(float l);
	float getLife();
	void subLife(float delta);
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
	float life;
	float radius;
	glm::vec3 force;
};

