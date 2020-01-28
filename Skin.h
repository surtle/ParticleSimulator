#pragma once
#include <vector>
#include "glm/detail/type_vec.hpp"
#include "Vertex.h"
#include "Model.h"
#include "Skeleton.h"

using namespace glm;

class Skin
{
public: 
	Skin();
	Skin(Skeleton* s);
	~Skin();

	bool load(const char* file);
	void update(glm::mat4 parent);
	void draw(const glm::mat4& viewMatr, uint shader);

private: 
	std::vector<Vertex *> vertices;
	std::vector<vec3> normals; 
	std::vector<uint> indices;
	std::vector<mat4> bindings;

	Model* skinModel;
	Skeleton* skeleton;
};

