
#include "Core.h"
#include "Particle.h"
#include "Model.h"
#include <ctime>

#pragma once

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem(); 

	void genParticles();
	int findUnusedParticle();
	void handleCollisions(Particle* p);
	void update();
	void reset();

	void loadBuffers();
	void draw(glm::mat4 viewMatr, uint shader);

	// adjustable constants
	glm::vec3 pos;
	float posVar;

	glm::vec3 vel;
	float velVar;

	float life, lifeVar;

	int pps;
	float particleRad;
	glm::vec3 gravity;
	glm::vec3 wind;
	float airDens;
	float drag;
	float elasticity;
	float friction;
private:
	static const int maxParticles = 10000;
	Particle* particles[maxParticles];
	Model* particleSystemModel;

	float lastTime;

	int lastUsedParticle = 0;
	std::vector<ModelVertex> verts;
	std::vector<uint> indices;
};

