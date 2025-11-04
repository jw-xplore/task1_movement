#pragma once
#include "Entity.h"

enum ESteeringBehavior
{
	Seek = 0
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
	SteeringTarget* target;
	float maxVelocity = 1;
	float targetRadius = 1;
	float timeToTarget = 0.25f;

	const char* const SPRITE = "ship";

public:
	Agent();
	~Agent();
	void Update(float dTime) override;
	void Draw() override;

	// Steering behavior
	void Seek();
	void Flee();
	void Pursue(float maxPrediction);
	void Evade(float maxPrediction);
	void Arrive();
	void Wander(float maxRotation);
};