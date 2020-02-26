#pragma once

#include "Core.h"
#include "Joint.h"

class Skeleton
{
public: 
	Skeleton(); 
	~Skeleton(); 

	void init();
	bool load(const char* file); 
	void update(glm::mat4 transform);
	void draw(const glm::mat4& viewProjMtx, uint shader);

	Joint* getRoot();
	void changeJoint();
	void changeDOF();
	void changeJointDOF(int flag);

	void setRot(int rotDOF, float val);

	void changeRot(int rotDOF, int flag);

	void reset(); 

private:
	Joint* root;
	
	int activeJoint;
	std::vector<Joint*> joints;
};

