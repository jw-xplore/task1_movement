#include "Agent.h"

Agent::Agent()
{
	this->position = { 100, 100 };
}

void Agent::Update(float dTime)
{
	this->target = Play::GetMousePos();

	this->Arrive();
	Entity::Update(dTime);
}

void Agent::Draw()
{
	this->rotation = atan2(this->velocity.y, this->velocity.x);
	DrawSpriteRotated(SPRITE, this->position, 0, this->rotation, 1);
}

/*
* Steering behavior
*/

void Agent::Seek()
{
	Point2D targetVelocity = this->target - this->position;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Flee()
{
	Point2D targetVelocity = this->position - this->target;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Arrive()
{
	Point2D targetVelocity = this->target - this->position;

	// Stop
	if (targetVelocity.Length() < targetRadius)
	{
		this->velocity = { 0, 0 };
		return;
	}

	// Adjust speed
	targetVelocity /= timeToTarget;
	if (targetVelocity.Length() > maxVelocity)
	{
		targetVelocity.Normalize();
		targetVelocity = targetVelocity * maxVelocity;
	}

	this->velocity = targetVelocity;
}