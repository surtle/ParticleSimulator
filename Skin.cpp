#include "Skin.h"
#include "Tokenizer.h"
#include <iostream>

Skin::Skin() {
	skinModel = new Model();
}

Skin::Skin(Skeleton* s)
{
	skinModel = new Model();
	skeleton = s;
}

Skin::~Skin()
{
	delete skinModel;
	delete skeleton;

	for (Vertex* v : vertices) {
		delete v;
	}
}

bool Skin::load(const char* file)
{
	Tokenizer token;
	token.Open(file);

	// load positions ==========================
	token.FindToken("positions");
	int numVertices = token.GetInt();
	token.FindToken("{");

	for (int i = 0; i < numVertices; i++) {
		Vertex * v = new Vertex();
		vec3 vertex;

		vertex.x = token.GetFloat();
		vertex.y = token.GetFloat();
		vertex.z = token.GetFloat();

		v->setPosition(vertex);

		vertices.push_back(v);
	}

	// load normals ==========================
	token.FindToken("normals");
	token.FindToken("{");

	for (int i = 0; i < numVertices; i++) {
		vec3 normal;

		normal.x = token.GetFloat();
		normal.y = token.GetFloat();
		normal.z = token.GetFloat();

		vertices[i]->setNormal(normal);
		normals.push_back(normal);
	}

	// load skin weights ==========================
	token.FindToken("skinweights");
	int numSkinWeights = token.GetInt();
	token.FindToken("{");

	for (int i = 0; i < numSkinWeights; i++) {
		int numJoints = token.GetInt();

		for (int j = 0; j < numJoints; j++) {
			int jointIndex = token.GetInt();
			float skinWeight = token.GetFloat();

			vertices[i]->addJoint(jointIndex);
			vertices[i]->addWeight(jointIndex, skinWeight);
		}
	}

	// load triangles ==========================
	token.FindToken("triangles");
	int numTriangles = token.GetInt();
	token.FindToken("{");

	for (int i = 0; i < numTriangles; i++) {
		vec3 index;

		indices.push_back(token.GetInt());
		indices.push_back(token.GetInt());
		indices.push_back(token.GetInt());
	}

	// load bindings ==========================
	token.FindToken("bindings");
	int numBindings = token.GetInt();
	token.FindToken("{"); 

	for (int i = 0; i < numBindings; i++) {
		token.FindToken("matrix");
		token.FindToken("{");

		glm::mat4 binding;

		for (int j = 0; j < 4; j++) {
			binding[j][0] = token.GetFloat();
			binding[j][1] = token.GetFloat();
			binding[j][2] = token.GetFloat();
		}

		// calculate inverse of binding matrix
		binding = inverse(binding);
		binding[3][3] = 1;

		bindings.push_back(binding);
	}

	token.Close();

	return false;
}

void Skin::update(glm::mat4 parent)
{
	glm::mat4 m;
	std::vector<glm::mat4> ms;
	std::vector<Vertex*> newVertices;

	std::vector<Joint*> joints = skeleton->getRoot()->getJoints();
	for (uint i = 0; i < joints.size(); i++) {
		// PRECALCULATE M
		m = joints[i]->getWorldMat() * bindings[i];
		ms.push_back(m);
	}

	// calculate blended vertices 
	for (Vertex* v : vertices) {
		vec4 oldPos = vec4(v->getPosition(), 1);
		vec3 newPos = glm::vec3(0);

		vec3 oldNorm = v->getNormal();
		vec3 newNorm = glm::vec3(0);

		for (int j : v->getJoints()) {

			mat3 m_mat3 = mat3(vec3(ms[j][0]), vec3(ms[j][1]), vec3(ms[j][2]));

			vec4 newP = v->getWeights()[j] * ms[j] * oldPos;
			vec3 newN = v->getWeights()[j] * m_mat3 * oldNorm;

			newPos += vec3(newP.x, newP.y, newP.z);
			newNorm += vec3(newN.x, newN.y, newN.z);
		}

		newNorm = normalize(newNorm);

		Vertex* newVert = new Vertex();

		newVert->setPosition(newPos);
		newVert->setNormal(newNorm);

		newVertices.push_back(newVert);

	}

	std::vector<ModelVertex> new_vertices;
	for (Vertex* v : newVertices) {
		new_vertices.push_back({ v->getPosition(), v->getNormal() });
	}

	// set buffers 
	skinModel->SetBuffers(new_vertices, indices);

	// delete the previously calculated blended vertices 
	for (Vertex* v : newVertices) {
		delete v;
	}
}

void Skin::draw(const glm::mat4& viewMatr, uint shader)
{
	glm::mat4 newProjMatr = viewMatr;
	skinModel->Draw(glm::mat4(1), viewMatr, shader);
}
