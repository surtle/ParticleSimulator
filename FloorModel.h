#pragma once
#include "Model.h"

class FloorModel :
	public Model
{
public:
	void FloorModel::Draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx, uint shader);
};

