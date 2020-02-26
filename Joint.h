#pragma once

#include "Core.h"
#include "Tokenizer.h"
#include "Model.h"
#include "DOF.h"

class Joint
{
public:
	Joint();
	~Joint(); 

	bool load(Tokenizer &t);
	void clamp();
	// load data from skel file 
	void update(glm::mat4 parent);		// use to update self and children 
	void draw(const glm::mat4& viewProjMtx, uint shader);						// draw self and recursively draw 
	void addChild(Joint* child);			// add child to children 

	glm::mat4 getLocalMat(); 
	glm::mat4 getWorldMat(); 

	void changeDOF(); 
	void changeDOFVal(int flag); 
	void setRot(int rotDOF, float val);
	void changeRot(int rotDOF, int flag);
	void setOffset(float x, float y, float z);

	static std::vector<Joint*> getJoints();

private:
	static std::vector<Joint*> joints;

	glm::mat4 localMat;
	glm::mat4 worldMat;

	glm::vec3 offset; 
	glm::vec3 boxmin;
	glm::vec3 boxmax;
	glm::vec3 pose;

	Model* jointModel; 

	int activeDOF;
	std::vector<DOF *> dofs; 

	std::vector<Joint *> children; 
};

