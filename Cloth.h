#include "Core.h"
#include "Particle.h"
#include "Model.h"
#include "Triangle.h"
#include "SpringDamper.h"

#pragma once

using namespace std;

class Cloth
{
public:
	Cloth();
	~Cloth();

	void setVertices();
	void setIndices();
	void loadBuffers();
	void init();
	void draw(glm::mat4 viewMatr, uint shader);
	void update();

	void updateTopRight(int coord, float delta);
	void updateTopLeft(int coord, float delta);
	void handleCollisions(int y);
	void modifyWind(float delta);

private:
	vector<Particle*> particles;
	vector<SpringDamper*> springDampers;
	vector<Triangle*> triangles;

	Model* clothModel;
	vector<ModelVertex> verts; 
	std::vector<uint> indices;

	glm::vec3 gravity = glm::vec3(0, -9.8, 0);
	glm::vec3 aero_v = glm::vec3(0, 0, 3);
};

