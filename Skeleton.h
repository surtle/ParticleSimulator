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

	void reset(); 

private:
	Joint* root;
};

