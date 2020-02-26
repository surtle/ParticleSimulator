#include "Channel.h"
#include <iostream>

#define jason {I, Love , You}

Channel::Channel()
{
	Channel::extrapIn = CONSTANT;
	Channel::extrapOut = CONSTANT;
}

Channel::~Channel()
{
	for (Keyframe* k : keyframes) {
		delete k;
	}
}

void Channel::setExtrapIn(Extrapolation e)
{
	extrapIn = e;
}

void Channel::setExtrapOut(Extrapolation e)
{
	extrapOut = e;
}

void Channel::addKeyframe(Keyframe* kf)
{
	keyframes.push_back(kf);
}

void Channel::precompute()
{	
	// precompute tangents
	calculateTangents();

	// precompute cubics
	calculateCubics();
}

void Channel::calculateCubics()
{
	for (int i = 0; i < keyframes.size() - 1; i++) {
		// keyframe values 
		float p0 = keyframes[i]->getValue();
		float p1 = keyframes[i + 1]->getValue();

		// keyframe tangents 
		float v0 = keyframes[i]->getTanOut();
		float v1 = keyframes[i + 1]->getTanIn();

		// keyframe times 
		float t0 = keyframes[i]->getTime();
		float t1 = keyframes[i + 1]->getTime();

		float a = (2 * p0) - (2 * p1) + ((t1 - t0) * v0) + ((t1 - t0) * v1);
		float b = (-3 * p0) + (3 * p1) - (2 * (t1 - t0) * v0) - (t1 - t0) * v1;
		float c = (t1 - t0) * v0;
		float d = p0;

		keyframes[i]->setCubics(a, b, c, d);

		//glm::vec4 g = glm::vec4(p0, p1, (t1 - t0) * v0,  (t1 - t0) * v1);
		//glm::vec4 g = glm::vec4(a, b, c, d);

		// calculate cubics and save
		//glm::vec4 cubics = hermite * g;
	}
}

void Channel::calculateTangents()
{
	for (int i = 0; i < keyframes.size(); i++) {

		// compute tangent in
		switch (keyframes[i]->getTanIn()) {
		case FIXED_T:
			break;
		case FLAT:
			keyframes[i]->setInVal(0);
			break;
		case SMOOTH:
			// calculate linear slope if first or last point
			if (i == 0 && keyframes.size() > 1) {
				float tan = (keyframes[i + 1]->getValue() - keyframes[i]->getValue()) / (keyframes[i + 1]->getTime() - keyframes[i]->getTime());
				keyframes[i]->setInVal(tan);
			}
			else if (i == 0 && keyframes.size() == 1) {
				keyframes[i]->setInVal(0);
			}
			else if (i == keyframes.size() - 1) {
				float tan = (keyframes[i]->getValue() - keyframes[i - 1]->getValue()) / (keyframes[i]->getTime() - keyframes[i - 1]->getTime());
				keyframes[i]->setInVal(tan);
			}

			// (p2 - p0) / (t2 - t0) 
			else {
				float tan = (keyframes[i + 1]->getValue() - keyframes[i - 1]->getValue()) / (keyframes[i + 1]->getTime() - keyframes[i - 1]->getTime());
				keyframes[i]->setInVal(tan); 
			}
			break;
		case LINEAR_T:
			if (i > 0) {
				float tan = (keyframes[i]->getValue() - keyframes[i - 1]->getValue()) / (keyframes[i]->getTime() - keyframes[i - 1]->getTime());
				keyframes[i]->setInVal(tan); 
			}
			break;
		default:
			break;
		}

		cout << "tangent in: " << keyframes[i]->getInVal() << endl;

		// compute tangent out
		switch (keyframes[i]->getTanOut()) {
		case FIXED_T:
			break;
		case FLAT:
			keyframes[i]->setOutVal(0);
			break;
		case SMOOTH:

			// calculate linear slope if first or last point
			if (i == 0 && keyframes.size() > 1) {
				float tan = (keyframes[i + 1]->getValue() - keyframes[i]->getValue()) / (keyframes[i + 1]->getTime() - keyframes[i]->getTime());
				keyframes[i]->setOutVal(tan);
			}
			else if (i == 0 && keyframes.size() == 1) {
				keyframes[i]->setOutVal(0);
			}
			else if (i == keyframes.size() - 1) {
				float tan = (keyframes[i]->getValue() - keyframes[i - 1]->getValue()) / (keyframes[i]->getTime() - keyframes[i - 1]->getTime());
				keyframes[i]->setOutVal(tan);
			}

			// (p2 - p0) /(t2 - t0) 
			else {
				float tan = (keyframes[i + 1]->getValue() - keyframes[i - 1]->getValue()) / (keyframes[i + 1]->getTime() - keyframes[i - 1]->getTime());
				keyframes[i]->setOutVal(tan);
			}

			break;
		case LINEAR_T:
			if (i < keyframes.size() - 1) {
				float tan = (keyframes[i + 1]->getValue() - keyframes[i]->getValue()) / (keyframes[i + 1]->getTime() - keyframes[i]->getTime());
				keyframes[i]->setOutVal(tan);
			}

			break;
		default:
			break;
		}

		cout << "tangent out: " << keyframes[i]->getOutVal() << endl;
	}
}

float Channel::evaluate(float time)
{
	// check extrapolation mode 
	Keyframe* kf = keyframes[keyframes.size() - 1];
	Keyframe* nextKF = keyframes[keyframes.size() - 1];

	if (keyframes.size() == 1) {
		return keyframes[0]->getValue();
	}
	else if (time < keyframes[0]->getTime()) {
		return evalExtrap(time, IN_T);
	}
	else if (time > keyframes[keyframes.size() - 1]->getTime()) {
		return evalExtrap(time, OUT_T);
	}
	else {

		// find span 
		for (int i = 0; i < keyframes.size() - 1; i++) {
			if (keyframes[i]->getTime() == time) {
				return keyframes[i]->getValue();
			}
			else if (keyframes[i]->getTime() <= time && keyframes[i + 1]->getTime() > time) {
				kf = keyframes[i];
				nextKF = keyframes[i + 1];
				break;
			}
		}

		float u = (time - kf->getTime()) / (nextKF->getTime() - kf->getTime());

		float x = (kf->A() * u * u * u) + (kf->B() * u * u) + (kf->C() * u) + kf->D();
		return x;
	}
}


float Channel::evalExtrap(float time, Direction dir)
{
	float totalTime, i, range;
	int numCycles = 0;

	totalTime = keyframes[keyframes.size() - 1]->getTime() - keyframes[0]->getTime();
	range = keyframes[keyframes.size() - 1]->getValue() - keyframes[0]->getValue();

	switch (dir) {
	case IN_T:
		switch (extrapIn) {
		case CONSTANT:
			return keyframes[0]->getValue();
			break;
		case LINEAR:
			// y - y1 = m(x - x1) -> val = m(time - t1) + p1; 
			return keyframes[0]->getTanOut() * (time - keyframes[0]->getTime()) + keyframes[0]->getValue();
			break;
		case CYCLE:
			while (time < keyframes[0]->getTime()) {
				time += totalTime;
			}
			return evaluate(time);

			break;
		case CYCLE_OFFSET:
			while (time < keyframes[0]->getTime()) {
				time += totalTime;
			}
			return evaluate(time) - (numCycles * range);

			break;
		case BOUNCE:
			if (numCycles % 2 == 0) {
				return evaluate(fmod(totalTime - (keyframes[0]->getTime() - time), totalTime) + keyframes[0]->getTime());
			}
			else {
				return evaluate(fmod(totalTime - (totalTime - (keyframes[0]->getTime() - time)), totalTime) + keyframes[0]->getTime());
			}
			break;
		}
		break;
	case OUT_T:
		switch (extrapOut) {
		case CONSTANT:
			return keyframes[keyframes.size() - 1]->getValue();
			break;
		case LINEAR:
			// y - y1 = m(x - x1) -> val = m(time - t1) + p1; 
			return keyframes[keyframes.size() - 1]->getTanIn() * (time - keyframes[keyframes.size() - 1]->getTime()) + keyframes[keyframes.size() - 1]->getValue();
			break;
		case CYCLE:
			while (time > keyframes[keyframes.size() - 1]->getTime()) {
				time -= totalTime;
			}
			return evaluate(time);

			break;
		case CYCLE_OFFSET:
			while (time > keyframes[keyframes.size() - 1]->getTime()) {
				time -= totalTime;
				numCycles++;
			}

			return evaluate(time) + (numCycles * range);

			break;
		case BOUNCE:
			if (numCycles % 2 == 0) {
				return evaluate(fmod(totalTime - (keyframes[0]->getTime() - time), totalTime) + keyframes[0]->getTime());
			}
			else {
				return evaluate(fmod(totalTime - (totalTime - (keyframes[0]->getTime() - time)), totalTime) + keyframes[0]->getTime());
			}
			break;
		}
		break;
	}
}
