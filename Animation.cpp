#include "Animation.h"
#include "Tokenizer.h"
#include <iostream>

Animation::Animation()
{
}

Animation::~Animation()
{
	for (Channel* c : channels) {
		delete c;
	}
}

void Animation::load(const char* file)
{
	Tokenizer token;
	token.Open(file);

	// load positions ==========================
	token.FindToken("animation");
	token.FindToken("{");

	token.FindToken("range");

	timeStart = token.GetFloat();
	timeEnd = token.GetFloat(); 

	token.FindToken("numchannels");
	int numChannels = token.GetInt();

	for (int i = 0; i < numChannels; i++) {
		Channel* channel = new Channel();
		token.FindToken("channel");

		char ei[256];
		char eo[256];
		token.FindToken("extrapolate");
		
		token.GetToken(ei);
		token.GetToken(eo);

		// set extrapolation in 
		if (strcmp(ei, "constant") == 0) {	
			channel->setExtrapIn(CONSTANT);
		}
		else if (strcmp(ei, "linear") == 0) {	
			channel->setExtrapIn(LINEAR);
		}
		else if (strcmp(ei, "cycle") == 0) {
			channel->setExtrapIn(CYCLE);
		}
		else if (strcmp(ei, "cycle_offset") == 0) {		
			channel->setExtrapIn(CYCLE_OFFSET);
		}
		else if (strcmp(ei, "bounce") == 0) {			
			channel->setExtrapIn(BOUNCE);
		}

		// set extrapolation out 
		if (strcmp(eo, "constant") == 0) {	
			channel->setExtrapOut(CONSTANT);
		}
		else if (strcmp(eo, "linear") == 0) {	
			channel->setExtrapOut(LINEAR);
		}
		else if (strcmp(eo, "cycle") == 0) {	
			channel->setExtrapOut(CYCLE);
		}
		else if (strcmp(eo, "cycle_offset") == 0) {	
			channel->setExtrapOut(CYCLE_OFFSET);
		}
		else if (strcmp(eo, "bounce") == 0) {
			channel->setExtrapOut(BOUNCE);
		}

		token.FindToken("keys");
		int numKeys = token.GetInt();
		token.FindToken("{");

		// save in keyframes 
		for (int i = 0; i < numKeys; i++) {

			float time = token.GetFloat();
			float value = token.GetFloat();

			Keyframe* keyframe = new Keyframe(time, value);

			char ti[256];
			char to[256];

			// get tangent in
			token.GetToken(ti);
			if (strcmp(ti, "flat") == 0) {
				keyframe->setTanIn(FLAT);
			}
			else if (strcmp(ti, "smooth") == 0) {
				keyframe->setTanIn(SMOOTH);
			}
			else if (strcmp(ti, "linear") == 0) {
				keyframe->setTanIn(LINEAR_T);
			}
			else {
				keyframe->setTanIn(FIXED_T);
				keyframe->setInVal(atof(ti));
			}

			// get tangent out
			token.GetToken(to);
			if (strcmp(to, "flat") == 0) {
				keyframe->setTanOut(FLAT);
			}
			else if (strcmp(to, "smooth") == 0) {
				keyframe->setTanOut(SMOOTH);
			}
			else if (strcmp(to, "linear") == 0) {
				keyframe->setTanOut(LINEAR_T);
			}
			else {
				keyframe->setTanOut(FIXED_T);
				keyframe->setOutVal(atof(to));
			}

			// add kf to list of keyframes 
			channel->addKeyframe(keyframe);
		}

		channels.push_back(channel);
	}

	token.Close();
}

void Animation::init()
{
	int i = 0;
	for (Channel* c : channels) {
		cout << "Channel " << i << endl;
		c->precompute();
		i++;
		cout << endl;
	}
}

vector<float> Animation::evaluate(float time)
{
	vector<float> values;
	for (Channel* c : channels) {
		values.push_back(c->evaluate(time));
	}

	return values;
}
