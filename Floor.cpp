#include "Floor.h"

Floor::Floor()
{
	floor = new Model();

	floor->SetBuffers(vertices, indices);
}

void Floor::draw(glm::mat4 viewMatr, uint shader)
{
	floor->Draw(glm::mat4(1), viewMatr, shader);
}
