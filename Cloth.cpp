#include "Cloth.h"
#include "Triangle.h"
#include <iostream>

using namespace std;

#define START -2
#define END 2
#define STEP 0.1

//#define WIDTH 20
//#define HEIGHT 20

int WIDTH = (END - START) / STEP;
int HEIGHT = (END - START) / STEP;

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
	for (float r = 2; r > -2; r -= STEP) { 
		for (float c = -2; c < 2; c += STEP) { 
			Particle* p = new Particle(glm::vec3(c, r, 0), 0.1);
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
	particles[WIDTH - 1]->setStatic(true);

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
	particles[WIDTH/2 + HEIGHT/2]->applyForce(glm::vec3(0, 0, 0.01));

	// apply gravity to all particles
	for (Particle* p : particles) {
		p->applyForce(glm::vec3(0, -0.01, 0));	// GRAVITY ================================
	}

	// apply spring damper forces to respective particles
	for (SpringDamper* s : springDampers) {
		s->computeForce();
	}

	// apply aerodynamic force
	for (Triangle* t : triangles) {
		t->applyAeroForce(aero_v, 0.1, 0.5);
	}

	// compute acceleration and update particles 
	for (Particle* p : particles) {
		if (!p->getStatic()) {
			p->computeAcceleration();
			p->update(0.01);
			p->resetForces();
		}

		p->resetNormal();
	}

	// handle collision w/ floor & y = -2
	handleCollisions(-2);

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
		particles[WIDTH - 1]->moveX(delta);
		break;
	case 1:
		particles[WIDTH - 1]->moveY(delta);
		break;
	case 2:
		particles[WIDTH - 1]->moveZ(delta);
		break;
	}
}

void Cloth::handleCollisions(int y)
{
	float elasticity = 0.5;
	float friction = 0.5;

	for (Particle* p : particles) {
		glm::vec3 pos = p->getPos();

		if (pos.y < y) {
			float new_y = 2 * y - pos.y;
			p->setPos(glm::vec3(pos.x, new_y, pos.z));

			float v_x = (1 - friction) * p->getVelocity().x;
			float v_y = -elasticity * p->getVelocity().y;
			float v_z = (1 - friction) * p->getVelocity().z;

			p->setVelocity(glm::vec3(v_x, v_y, v_z));
		}
	}
}

void Cloth::modifyWind(float delta)
{
	glm::vec3 old_aero = aero_v;

	aero_v = glm::vec3(old_aero.x, old_aero.y, old_aero.z + delta);
	cout << "Wind strength now " << old_aero.z + delta << endl;
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
