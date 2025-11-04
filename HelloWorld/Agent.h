#pragma once
#include "Entity.h"

enum ESteeringBehavior
{
	Seek = 0
};

class Agent : public Entity
{
private:
	Point2D target = { 300, 300 };
	float maxVelocity = 2;
	float targetRadius = 1;
	float timeToTarget = 0.25f;

	const char* const SPRITE = "ship";

public:
	Agent();
	void Update(float dTime) override;
	void Draw() override;

	// Steering behavior
	void Seek();
	void Flee();
	void Pursue();
	void Evade();
	void Arrive();
	void Wander();
};