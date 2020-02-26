#pragma once
#include "Keyframe.h"
#include "Core.h"
#include <vector>

using namespace std;

enum Extrapolation {CONSTANT, LINEAR, CYCLE, CYCLE_OFFSET, BOUNCE};
enum Direction { IN_T, OUT_T };

class Channel
{
public:
	Channel();
	~Channel();
	void setExtrapIn(Extrapolation e);
	void setExtrapOut(Extrapolation e);
	void addKeyframe(Keyframe* kf);

	void precompute();
	void calculateCubics();
	void calculateTangents();
	float evaluate(float time);

	float evalExtrap(float time, Direction dir);

private:
	Extrapolation extrapIn;
	Extrapolation extrapOut;

	std::vector<Keyframe *> keyframes;

	glm::mat4 hermite =
		glm::mat4(2, -3, 0, 1,
			-2, 3, 0, 0,
			1, -2, 1, 0,
			1, -1, 0, 0);
};

