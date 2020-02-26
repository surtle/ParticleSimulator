#include "Cloth.h"
#include "Triangle.h"
#include <iostream>

using namespace std;

#define WIDTH 20
#define HEIGHT 20

Cloth::Cloth()
{
	clothModel = new Model();
}

void Cloth::init()
{
	// p0   p1   p2   p3  ...  p19
	// p20  p21  p22  p23 ...  p39
	//  .    .    .    .        .
	//  .    .    .    .        .
	// p380 p381 p382 p383 ... p399

	// initialize array of particles to be 20x20 particles 
	for (float r = 1.25; r > -1.25; r -= 0.125) { 
		for (float c = -1.25; c < 1.25; c += 0.125) { 
			Particle* p = new Particle(glm::vec3(c, r, 0), 0.1);
			//p->applyForce(gravity);
			//p->computeAcceleration();
			particles.push_back(p);
		}
	}

	// initialize triangles & spring dampers
	for (int c = 1; c < WIDTH; c++) {
		for (int r = 1; r < HEIGHT; r++) {
			Particle* p1 = particles[(r - 1) * HEIGHT + c - 1];			// p1 - - - p2
			Particle* p2 = particles[(r - 1) * HEIGHT + c];				// |         |
			Particle* p3 = particles[r * WIDTH + c - 1];				// |         |
			Particle* p4 = particles[r * WIDTH + c];					// p3 - - - p4

			triangles.push_back(new Triangle(p1, p2, p3));
			triangles.push_back(new Triangle(p2, p4, p3));

			springDampers.push_back(new SpringDamper(p1, p2));
			springDampers.push_back(new SpringDamper(p2, p4));
			springDampers.push_back(new SpringDamper(p3, p4));
			springDampers.push_back(new SpringDamper(p1, p3));
			springDampers.push_back(new SpringDamper(p1, p4));
			springDampers.push_back(new SpringDamper(p2, p3));
		}
	}

	particles[0]->setStatic(true);
	particles[19]->setStatic(true);

	setVertices();
	setIndices();
	loadBuffers();
}

void Cloth::draw(glm::mat4 viewMatr, uint shader)
{
	clothModel->Draw(glm::mat4(1), viewMatr, shader);
}

void Cloth::update()
{
	particles[150]->applyForce(glm::vec3(0, 0, 0.1));

	// apply gravity to all particles
	for (Particle* p : particles) {
		p->applyForce(glm::vec3(0, -0.01, 0));	// gravity
	}

	// apply spring damper forces to respective particles
	for (SpringDamper* s : springDampers) {
		s->computeForce();
	}

	// compute acceleration and update particles 
	for (Particle* p : particles) {
		if (!p->getStatic()) {
			p->computeAcceleration();
			p->update(0.01);
			p->resetForces();
		}
	}

	// zero out all normals
	for (Particle* p : particles) {
		p->resetNormal();
	}

	// loop through all triangles and add triangle normals to the normals of each particle it intersects
	for (Triangle* t : triangles) {
		t->calculateNormal();
	}

	// normalize normals
	for (Particle* p : particles) {
		p->normalizeNormal();
	}

	setVertices();
	loadBuffers();
}

void Cloth::updateTopRight(int coord, float delta)
{
	switch (coord) {
	case 0:
		particles[0]->moveX(delta);
		break;
	case 1:
		particles[0]->moveY(delta);
		break;
	case 2:
		particles[0]->moveZ(delta);
		break;
	}
}

void Cloth::updateTopLeft(int coord, float delta)
{
	switch (coord) {
	case 0:
		particles[19]->moveX(delta);
		break;
	case 1:
		particles[19]->moveY(delta);
		break;
	case 2:
		particles[19]->moveZ(delta);
		break;
	}
}

void Cloth::setVertices()
{
	vector<ModelVertex> vertices;

	for (int i = 0; i < triangles.size(); i++) {
		Particle* p1 = triangles[i]->getP(1);
		Particle* p2 = triangles[i]->getP(2);
		Particle* p3 = triangles[i]->getP(3);
		
		vertices.push_back({ p1->getPos(), p1->getNormal() });
		vertices.push_back({ p2->getPos(), p2->getNormal() });
		vertices.push_back({ p3->getPos(), p3->getNormal() });
	}

	verts = vertices;
}

void Cloth::setIndices()
{
	for (int i = 0; i < verts.size(); i++) {
		indices.push_back(i);
	}
}

void Cloth::loadBuffers()
{
	clothModel->SetBuffers(verts, indices);
}
