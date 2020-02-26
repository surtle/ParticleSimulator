#pragma once
#include "Animation.h"
#include "Skeleton.h"

class Player
{
public:
	Player();
	Player(Animation* a, Skeleton* s);
	~Player();

	void init();
	void update();
	void addAnim(Animation* a);
	void addSkel(Skeleton* s);

private:
	Animation* anim;
	Skeleton* skel; 

	float time;
};

