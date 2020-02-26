#include "Player.h"
#include <iostream>

using namespace std;

float z_trans = 0;

Player::Player()
{
	time = 0;
}

Player::Player(Animation* a, Skeleton* s)
{
	time = 0;
	anim = a;
	skel = s;
}

Player::~Player()
{
	delete anim;
	delete skel;
}

void Player::init()
{
	anim->init();
}

void Player::update()
{
	// increment time
	time += 0.001; 

	// eval animation
	vector<float> newDOFvals = anim->evaluate(time);

	// init root translation
	skel->getRoot()->setOffset(newDOFvals[0], newDOFvals[1], newDOFvals[2]);

	// update rig 
	for (int i = 3; i < newDOFvals.size() - 2; i += 3) {
		skel->setRot(ROTX, newDOFvals[i]);
		skel->setRot(ROTY, newDOFvals[i + 1]);
		skel->setRot(ROTZ, newDOFvals[i + 2]);

		skel->changeJoint();
	}
}

void Player::addAnim(Animation* a)
{
	anim = a;
}

void Player::addSkel(Skeleton* s)
{
	skel = s;
}
