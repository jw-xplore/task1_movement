#pragma once
#include "vector"
#include "Path.h"
#include "Play.h"

struct Collision
{
public:
	Play::Point2D position;
	Play::Point2D normal;
};

class CollisionHandler
{
public:
	//std::vector<Path*> walls;

	Collision detectCollision(std::vector<Path*> walls, Play::Point2D pos);
};

