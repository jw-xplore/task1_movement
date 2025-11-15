#pragma once
#include "Entity.h"
#include "Path.h"
#include "SteeringBehavior.h"

enum ESteeringBehavior
{
	Seek = 1,
	Flee = 2,
	Arrive = 4,
	Pursue = 8,
	Evade = 16,
	Wander = 32
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
	ESteeringBehavior steeringType = ESteeringBehavior::Seek;
	SteerTarget* target;
	SteerTarget* predictTarget;
	float targetRadius = 1;
	float timeToTarget = 0.1f;
	SteeringBehavior* steeringBehavior;

	const char* const SPRITE = "ship";

public:
	float maxVelocity = 80;
	float maxAcceleration = 100;
	CollisionHandler* collisionHandler;
	std::vector<Path*> walls;

	Agent();
	Agent(Point2D startPos, SteeringBehavior* steeringBeh);
	~Agent();
	void Update(float dTime) override;
	void Draw() override;

	// Steering behavior
	void Steer();
	void Seek();
	void Flee();
	void Pursue(float maxPrediction);
	void Evade(float maxPrediction);
	void Arrive();
	void Wander(float maxRotation);
	void FollowPath(Path path, int offset);
};