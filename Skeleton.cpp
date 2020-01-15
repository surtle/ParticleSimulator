#include "Skeleton.h"
#include "Tokenizer.h"

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
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

void Skeleton::reset()
{
}
