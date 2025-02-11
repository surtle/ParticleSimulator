////////////////////////////////////////
// Model.h
////////////////////////////////////////

#pragma once

#include "Core.h"
#include "Vertex.h"

////////////////////////////////////////////////////////////////////////////////

struct ModelVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
};

////////////////////////////////////////////////////////////////////////////////

class Model {
public:
	Model();
	~Model();

	void Draw(const glm::mat4 &modelMtx,const glm::mat4 &viewProjMtx,uint shader);

	void MakeBox(const glm::vec3 &boxMin,const glm::vec3 &boxMax);
	void SetBuffers(const std::vector<ModelVertex> &vtx,const std::vector<uint> &idx);
	void SetBuffers(std::vector<vec3> vertices, std::vector<int> indices);

	// Access functions

protected:
	uint VertexBuffer;
	uint IndexBuffer;

	int Count;
};

////////////////////////////////////////////////////////////////////////////////
