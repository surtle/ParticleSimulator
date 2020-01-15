#pragma once

#include "Core.h"
#include <string>

class DOF
{
public:
	DOF(char* name); 
	~DOF(); 

	char* getName();

	void setValue(float val);
	float getValue();

	void setMinMax(float min, float max); 
	float getMin();
	float getMax();

private:
	char* name;
	float value;
	float min;
	float max;
};

