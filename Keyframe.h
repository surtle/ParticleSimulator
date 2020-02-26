#pragma once
#include "Core.h"

enum Tangent {FLAT, SMOOTH, LINEAR_T, FIXED_T};

class Keyframe
{
public:
	Keyframe();
	Keyframe(float t, float v);
	~Keyframe();

	void setTanIn(Tangent in);
	void setTanOut(Tangent out);
	void setInVal(float in);
	void setOutVal(float out);
	void setCubics(float x, float y, float z, float w);

	Tangent getTanIn();
	Tangent getTanOut();
	float getInVal();
	float getOutVal();
	float A();
	float B();
	float C();
	float D();

	float getTime();
	float getValue(); 

private:
	float time;
	float value;
	float tan_in_val;
	float tan_out_val;
	float a, b, c, d;

	Tangent tan_in;
	Tangent tan_out;
};

