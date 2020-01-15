#include "DOF.h"

DOF::DOF(char* name)
{
	this->name = name;
}

DOF::~DOF()
{
}

char* DOF::getName()
{
	return this->name;
}

void DOF::setValue(float val)
{
	this->value = val; 
}

float DOF::getValue()
{
	return this->value;
}

void DOF::setMinMax(float min, float max)
{
	this->min = min;
	this->max = max;
}

float DOF::getMin()
{
	return min;
}

float DOF::getMax()
{
	return max;
}
