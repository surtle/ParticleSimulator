#include "Skeleton.h"
#include "Tokenizer.h"
#include <iostream>
using namespace std;

Skeleton::Skeleton()
{
	activeJoint = 0;
}

Skeleton::~Skeleton()
{
	for (Joint* j : joints) {
		delete j;
	}
}

void Skeleton::init()
{
}

bool Skeleton::load(const char* file)
{
	Tokenizer token; 
	token.Open(file); 
	token.FindToken("balljoint"); 

	root = new Joint(); 
	root->load(token); 

	joints = root->getJoints();
	cout << "num joints: " << joints.size() << endl;

	token.Close();
	return true;
}

void Skeleton::update(glm::mat4 transform)
{
	root->update(transform);
}

void Skeleton::draw(const glm::mat4& viewProjMtx, uint shader)
{
	root->draw(viewProjMtx, shader);
}

Joint* Skeleton::getRoot()
{
	return root;
}

void Skeleton::changeJoint()
{
	if (activeJoint == joints.size() - 1) {
		activeJoint = 0;
	}
	else {
		activeJoint++;
	}

	//cout << "Selected Joint #" << activeJoint << endl;
}

void Skeleton::changeDOF()
{
	joints[activeJoint]->changeDOF();
}

void Skeleton::changeJointDOF(int flag)
{
	if (flag == -1) {
		// decrease DOF
		joints[activeJoint]->changeDOFVal(-1);
	}
	else if (flag == 1) {
		// increase DOF
		joints[activeJoint]->changeDOFVal(1);
	}
}

void Skeleton::setRot(int rotDOF, float val) 
{
	joints[activeJoint]->setRot(rotDOF, val);
}

void Skeleton::changeRot(int rotDOF, int flag)
{
	joints[activeJoint]->changeRot(rotDOF, flag);
}

void Skeleton::reset()
{
}
