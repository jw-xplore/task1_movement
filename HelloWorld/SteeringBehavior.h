#pragma once
#include "Play.h"
#include "Path.h"
#include "vector"
#include "Entity.h"
#include "CollisionHandler.h"

using namespace Play;

struct SteerTarget
{
public:
	Point2D position = { 0,0 };
	Point2D prevPosition = { 0,0 };
	Point2D velocity = { 0,0 };
};

class SteeringBehavior
{
public:
	std::vector<Entity*> separationObstacles;

	Point2D seek(SteerTarget* target, Point2D pos, float maxAccel);
	Point2D flee(SteerTarget* target, Point2D pos, float maxAccel);
	Point2D arrive(SteerTarget* target, Point2D pos, Point2D vel, float maxVel, float maxAccel, float radius, float timeToTarget);
	SteerTarget* predictTarget(SteerTarget* target, Point2D pos, Point2D vel, float maxPredict);
	SteerTarget* followPath(Path* path, Point2D pos, Point2D vel, int offset);
	Point2D separate(std::vector<Entity*> obstacles, Entity* self, Point2D pos, float threshold, float maxAccel);
	Point2D avoidCollisions(std::vector<Entity*> obstacles, Entity* self, float radius, float maxAccel);
	SteerTarget* avoidObstacles(CollisionHandler* collisiions, std::vector<Path*> walls, Entity* self, float avoidDist, float lookahead);
};