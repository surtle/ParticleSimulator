#pragma once
#include "Channel.h"

class Animation
{
public:
	Animation();
	~Animation();
	void load(const char* file);

	void init();
	vector<float> evaluate(float time);
private:
	std::vector<Channel *> channels;

	float timeStart;
	float timeEnd;
};

