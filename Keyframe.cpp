#include "Keyframe.h"

Keyframe::Keyframe()
{
	tan_in_val = 0;
	tan_out_val = 0;
}

Keyframe::Keyframe(float t, float v)
{
	time = t;
	value = v;
	tan_in = FLAT;
	tan_out = FLAT;
	tan_in_val = 0;
	tan_out_val = 0;
}

Keyframe::~Keyframe()
{
}

void Keyframe::setTanIn(Tangent in)
{
	tan_in = in;
}

void Keyframe::setTanOut(Tangent out)
{
	tan_out = out;
}

void Keyframe::setInVal(float in)
{
	tan_in_val = in;
}

void Keyframe::setOutVal(float out)
{
	tan_out_val = out;
}

void Keyframe::setCubics(float x, float y, float z, float w)
{
	a = x;
	b = y;
	c = z;
	d = w;
}

Tangent Keyframe::getTanIn()
{
	return tan_in;
}

Tangent Keyframe::getTanOut()
{
	return tan_out;
}

float Keyframe::getInVal()
{
	return tan_in_val;
}

float Keyframe::getOutVal()
{
	return tan_out_val;
}

float Keyframe::A()
{
	return a;
}

float Keyframe::B()
{
	return b;
}

float Keyframe::C()
{
	return c;
}

float Keyframe::D()
{
	return d;
}

float Keyframe::getTime()
{
	return time;
}

float Keyframe::getValue()
{
	return value;
}
