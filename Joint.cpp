#include "Joint.h"
#include <iostream>

std::vector<Joint*> Joint::joints;

Joint::Joint()
{
	localMat = glm::mat4(1);
	worldMat = glm::mat4(1);

	offset = glm::vec3(0);
	boxmin = glm::vec3(-0.1);
	boxmax = glm::vec3(0.1);
	pose = glm::vec3(0);

	jointModel = new Model();

	Joint::joints.push_back(this);
}

Joint::~Joint()
{
	delete jointModel; 

	for (DOF* dof : dofs) {
		delete dof;
	}
}

bool Joint::load(Tokenizer& t)
{
	t.FindToken("{");

	while (1) {
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {				// OFFSET
			offset.x = t.GetFloat();
			offset.y = t.GetFloat();
			offset.z = t.GetFloat();

			std::cout << "offset: " << offset.x << " " << offset.y << " " << offset.z << "\n"; 
		}
		else if (strcmp(temp, "boxmin") == 0) {			// BOX MIN
			boxmin.x = t.GetFloat();
			boxmin.y = t.GetFloat();
			boxmin.z = t.GetFloat();

			std::cout << "boxmin: " << boxmin.x << " " << boxmin.y << " " << boxmin.z << "\n";
		}
		else if (strcmp(temp, "boxmax") == 0) {			// BOX MAX
			boxmax.x = t.GetFloat();
			boxmax.y = t.GetFloat();
			boxmax.z = t.GetFloat();

			std::cout << "boxmax: " << boxmax.x << " " << boxmax.y << " " << boxmax.z << "\n";
		}
		else if (strcmp(temp, "rotxlimit") == 0) {		// ROT X LIMIT
			DOF* rotx = new DOF("rotx"); 
			float min = t.GetFloat();
			float max = t.GetFloat();

			rotx->setMinMax(min, max);
			dofs.push_back(rotx); 

			std::cout << "rotx" << min << " " << max << "\n"; 
		}
		else if (strcmp(temp, "rotylimit") == 0) {		// ROT Y LIMIT
			DOF* roty = new DOF("roty");
			float min = t.GetFloat();
			float max = t.GetFloat(); 

			roty->setMinMax(min, max);
			dofs.push_back(roty); 

			std::cout << "roty" << min << " " << max << "\n";
		}
		else if (strcmp(temp, "rotzlimit") == 0) {		// ROT Z LIMIT
			DOF* rotz = new DOF("rotz");
			float min = t.GetFloat();
			float max = t.GetFloat();

			rotz->setMinMax(min, max);
			dofs.push_back(rotz);  

			std::cout << "rotz" << min << " " << max << "\n";
		}
		else if (strcmp(temp, "pose") == 0) {			// POSE VALUES
			pose.x = t.GetFloat();
			pose.y = t.GetFloat();
			pose.z = t.GetFloat();

			std::cout << "pose: " << pose.x << " " << pose.y << " " << pose.z << "\n";
		}
		else if (strcmp(temp, "balljoint") == 0) {		// INIT NEW BALLJOINT
			std::cout << " adding new joint\n";
			Joint* jnt = new Joint();
			jnt->load(t);
			this->addChild(jnt);
		}

		else if (strcmp(temp, "}") == 0) {
			return true;
		}
		else {
			t.SkipLine();
		}
	}
}

void Joint::update(glm::mat4 parent)
{
	// local = for this joint
	// world = this joint * parent matrix 
	localMat = glm::mat4(1);
	glm::mat4 rotMat = glm::mat4(1);

	// loop through DOFs to clamp values if necessary
	for (DOF* dof : dofs) {
		if (strcmp(dof->getName(), "rotx") == 0) {
			if (pose.x < dof->getMin()) {			
				pose.x = dof->getMin();
			}
			else if (pose.x > dof->getMax()) {
				pose.x = dof->getMax();
			}
		}
		else if (strcmp(dof->getName(), "roty") == 0) {
			if (pose.y < dof->getMin()) {
				pose.y = dof->getMin();
			}
			else if (pose.y > dof->getMax()) {
				pose.y = dof->getMax();
			}
		}
		else if (strcmp(dof->getName(), "rotz") == 0) {
			if (pose.z < dof->getMin()) {
				pose.z = dof->getMin();
			}
			else if (pose.z > dof->getMax()) {
				pose.z = dof->getMax();
			}
		}
	}

	// perform DOF rotations
	rotMat = glm::rotate(rotMat, pose.z, glm::vec3(0, 0, 1));
	rotMat = glm::rotate(rotMat, pose.y, glm::vec3(0, 1, 0));
	rotMat = glm::rotate(rotMat, pose.x, glm::vec3(1, 0, 0));
	

	// calculate local and world matrices 
	localMat = glm::translate(offset) * rotMat; 
	worldMat = parent * localMat; 

	jointModel->MakeBox(boxmin, boxmax);

	for (Joint* j : children) {
		j->update(worldMat);
	}
}

void Joint::draw(const glm::mat4& viewProjMtx, uint shader)
{
	glm::mat4 newProjMatr = viewProjMtx * worldMat; 
	jointModel->Draw(glm::mat4(1), newProjMatr, shader);

	for (Joint* j : children) {
		j->draw(viewProjMtx, shader);
	}
}

void Joint::addChild(Joint* child)
{
	children.push_back(child);
}

glm::mat4 Joint::getLocalMat()
{
	return glm::mat4(1);
}

glm::mat4 Joint::getWorldMat()
{
	return worldMat;
}

std::vector<Joint*> Joint::getJoints()
{
	return joints;
}
