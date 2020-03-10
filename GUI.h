#include "Core.h"
#include "ParticleSystem.h"

#pragma once
class GUI
{
public:
	GUI(int x, int y, ParticleSystem* ps);
	~GUI();
	void init();
	void draw();

private:
	TwBar* sideBar;
	ParticleSystem* p;
};

