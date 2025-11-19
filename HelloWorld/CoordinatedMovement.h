#pragma once
#include <vector>
#include "Play.h"
#include "Entity.h"

class CoordinatedMovement
{
public:
	Entity* leader;
	std::vector<Entity*> members;
	float distance = 50;
	float angle = 0;

	Play::Point2D memberPosition(Entity* member);
};

