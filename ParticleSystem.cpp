#include "ParticleSystem.h"
#include <iostream>

using namespace std;

ParticleSystem::ParticleSystem()
{
	particleSystemModel = new Model();
	lastTime = std::clock();
	
	// initialize adjustable constants with defaults 
	pos = glm::vec3(0, 2, 0);
	posVar = 0;
	vel = glm::vec3(0.2, 0.2, 0);
	velVar = 1;
	life = 20; 
	lifeVar = 1;
	pps = 75;
	particleRad = 5.0f;
	gravity = glm::vec3(0, -7, 0); 
	wind = glm::vec3(0, 0, 0);
	airDens = 0.1;
	drag = 0.1;
	elasticity = 0;
	friction = 0;

	for (int i = 0; i < maxParticles; i++) {
		particles[i] = new Particle();
	}
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particles) {
		delete p;
	}

	delete particleSystemModel;
}

void ParticleSystem::genParticles()
{
	float deltaTime = std::clock() - lastTime;
	int newparticles = (int)(deltaTime * pps);
	if (newparticles > (int)(0.1 * pps)) {
		newparticles = (int)(0.1 * pps);
	}
	else if (newparticles == 0) {
		return;
	} 

	lastTime = std::clock();

	for (int i = 0; i < newparticles; i++) {
		int nextIndex = findUnusedParticle();
		delete particles[nextIndex];

		// calculate pos variance
		float r1 = (rand() % 15) / 45.0 * posVar;
		float r2 = (rand() % 15) / 45.0 * posVar;
		float r3 = (rand() % 15) / 45.0 * posVar;

		particles[nextIndex] = new Particle(glm::vec3(r1 + pos.x, r2 + pos.y, r3 + pos.z), 10, particleRad);

		// calculate vel variance 
		float v1 = (rand() % 15) / 45.0 * velVar;
		float v2 = (rand() % 15) / 45.0 * velVar;
		float v3 = (rand() % 15) / 45.0 * velVar;

		// calculate life variance
		float lv = (rand() % 15) / 45.0 * lifeVar;

		particles[nextIndex]->resetLife(life + lv); //reset life value
		particles[nextIndex]->setVelocity(glm::vec3(v1 + vel.x, v2 + vel.y, v3 + vel.z));		// set initial velocity
		particles[nextIndex]->applyForce(gravity);	// apply gravity
		particles[nextIndex]->computeAero(wind, airDens, drag);	// apply aero force
		particles[nextIndex]->resetLife(life);		// reset life value
	}
}

int ParticleSystem::findUnusedParticle()
{
	for (int i = lastUsedParticle; i < maxParticles; i++) {
		if (particles[i]->getLife() <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (particles[i]->getLife() <= 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	return 0; // All particles are taken, override the first one
}

void ParticleSystem::handleCollisions(Particle* p)
{
	int floor = -1;
	glm::vec3 pos = p->getPos();
	if (pos.y < floor) {					// floor is y = 0;
		float new_y = 2 * floor - pos.y;
		p->setPos(glm::vec3(pos.x, new_y, pos.z));

		float v_x = (1 - friction) * p->getVelocity().x;
		float v_y = -elasticity * p->getVelocity().y;
		float v_z = (1 - friction) * p->getVelocity().z;

		p->setVelocity(glm::vec3(v_x, v_y, v_z));
	}
}

void ParticleSystem::update()
{
	glPointSize(particleRad);
	genParticles();

	// Simulate all particles
	std::vector<ModelVertex> vertices;
	std::vector<uint> ind;

	for (int i = 0; i < maxParticles; i++) {

		Particle* p = particles[i]; // shortcut

		if (p->getLife() > 0.0f) {

			// Decrease life
			p->subLife(0.1);
			if (p->getLife() > 0.0f) {

				// Simulate simple physics
				p->computeAcceleration();
				p->update(0.02);
				handleCollisions(p);
				
				vertices.push_back({ p->getPos(), normalize(p->getVelocity()) });
				ind.push_back(i);
			}
		}
	}
	
	verts = vertices;
	indices = ind;
	loadBuffers();
}

void ParticleSystem::reset()
{
	// initialize adjustable constants with defaults 
	pos = glm::vec3(0, 2, 0);
	posVar = 0;
	vel = glm::vec3(0.2, 0.2, 0);
	velVar = 1;
	life = 20;
	lifeVar = 1;
	pps = 75;
	particleRad = 5.0f;
	gravity = glm::vec3(0, -7, 0);
	wind = glm::vec3(0, 0, 0);
	airDens = 0.1;
	drag = 0.1;
	elasticity = 0;
	friction = 0;
}

void ParticleSystem::loadBuffers()
{
	particleSystemModel->SetBuffers(verts, indices);
}

void ParticleSystem::draw(glm::mat4 viewMatr, uint shader)
{
	particleSystemModel->Draw(glm::mat4(1), viewMatr, shader);
}
