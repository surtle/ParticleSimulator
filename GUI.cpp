#include "GUI.h"
#include <iostream>

ParticleSystem* p2;

GUI::GUI(int x, int y, ParticleSystem* ps)
{
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(x, y);
	sideBar = TwNewBar("Options");

	p = ps;
	p2 = ps;

	init(); 
}

GUI::~GUI()
{
}

void TW_CALL callbck(void* clientData) {
	p2->reset();
}

void GUI::init()
{

	TwStructMember Vector3F[] = {
		{"X", TW_TYPE_FLOAT, offsetof(vec3, x), " Step=0.01 " },
		{"Y", TW_TYPE_FLOAT, offsetof(vec3, y), " Step=0.01 " },
		{"Z", TW_TYPE_FLOAT, offsetof(vec3, z), " Step=0.01 " },
	};

	TwType TW_VEC3;
	TW_VEC3 = TwDefineStruct("vec3", Vector3F, 3, sizeof(vec3), NULL, NULL);

	// add variables
	// particle creation rate
	TwAddVarRW(sideBar, "PPF", TW_TYPE_INT32, &p->pps, " Min=10 Max=100 Step=1 ");

	// initial lifespan 
	TwAddVarRW(sideBar, "Lifespan", TW_TYPE_FLOAT, &p->life, " Min=0 Step=0.1 ");
	TwAddVarRW(sideBar, "LifeVar", TW_TYPE_FLOAT, &p->lifeVar, " Min=0 Max=3 Step=0.01 ");

	// initial position
	TwAddVarRW(sideBar, "Position", TW_VEC3, &p->pos, NULL);					
	TwAddVarRW(sideBar, "Position Variance", TW_TYPE_FLOAT, &p->posVar, " Min=0 Max=3 Step=0.01 ");

	// initial velocity
	TwAddVarRW(sideBar, "Velocity", TW_VEC3, &p->vel, NULL);
	TwAddVarRW(sideBar, "Velocity Variance", TW_TYPE_FLOAT, &p->velVar, " Min=0 Max=3 Step=0.01 ");

	// gravity
	TwAddVarRW(sideBar, "Gravity", TW_VEC3, &p->gravity, NULL);			

	// aero force
	TwAddVarRW(sideBar, "Wind", TW_VEC3, &p->wind, NULL);

	// air density
	TwAddVarRW(sideBar, "Air density", TW_TYPE_FLOAT, &p->airDens, " Min=0 Step=0.01 ");

	// drag coefficient
	TwAddVarRW(sideBar, "Drag", TW_TYPE_FLOAT, &p->drag, " Min=0 Step=0.01 ");

	// radius
	TwAddVarRW(sideBar, "Particle radius", TW_TYPE_FLOAT, &p->particleRad, " Min=1 Max=20 Step=0.01 ");

	// elasticity
	TwAddVarRW(sideBar, "Collision elasticity", TW_TYPE_FLOAT, &p->elasticity, " Min=0 Step=0.01 ");

	// friction
	TwAddVarRW(sideBar, "Friction", TW_TYPE_FLOAT, &p->friction, " Min=0 Max=2 Step=0.01 ");

	// reset button
	TwAddButton(sideBar, "Reset", (TwButtonCallback)callbck, NULL, "");
}

void GUI::draw()
{
	TwDraw();
}




