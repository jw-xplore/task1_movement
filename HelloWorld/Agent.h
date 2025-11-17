#pragma once
#include "Entity.h"
#include "Path.h"
#include "SteeringBehavior.h"

enum ESteeringBehavior
{
	None,
	Seek,
	Flee,
	Arrive,
	Pursue,
	Evade,
	Wander,
	FollowPath,
	Separation,
	CollisionAvoidance,
	WallAvoidance,
	SteeringBehaviorCount
};

struct SteeringTarget
{
public:
	Point2D position = { 0,0 };
	Point2D prevPosition = { 0,0 };
	Point2D velocity = { 0,0 };
};

class Agent : public Entity
{
private:
	SteerTarget* target;
	SteerTarget* predictTarget;
	float targetRadius = 1;
	float timeToTarget = 0.1f;
	SteeringBehavior* steeringBehavior;

	const char* const SPRITE = "ship";

public:
	ESteeringBehavior steeringType = ESteeringBehavior::Seek;
	float maxVelocity = 60;
	float maxAcceleration = 100;
	CollisionHandler* collisionHandler;
	std::vector<Path*> walls;
	Path* followPath;

	Agent();
	Agent(Point2D startPos, SteeringBehavior* steeringBeh, CollisionHandler* collisionHandler);
	~Agent();
	void Update(float dTime) override;
	void Draw() override;

	// Steering behavior
	void Steer();
	void Wander(float maxRotation);
};