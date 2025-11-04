#include "Agent.h"

Agent::Agent()
{
	this->position = { 100, 100 };

	// Setup target
	this->target = new SteeringTarget();
	this->target->position = Play::GetMousePos();
	this->target->prevPosition = Play::GetMousePos();
}

Agent::~Agent()
{
	delete this->target;
}

void Agent::Update(float dTime)
{
	// Update target position
	this->target->position = Play::GetMousePos();
	this->target->velocity = this->target->position - this->target->prevPosition;
	this->target->prevPosition = this->target->position;

	// Update agent
	//this->Arrive();
	//this->Evade(2);
	this->Wander(5);
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
	Point2D targetVelocity = this->target->position - this->position;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Flee()
{
	Point2D targetVelocity = this->position - this->target->position;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Pursue(float maxPrediction)
{
	Point2D direction = this->target->position - this->position;
	float distance = direction.Length();

	float speed = this->velocity.Length();
	float prediction;

	if (speed <= distance / maxPrediction)
	{
		prediction = maxPrediction;
	}
	else
	{
		prediction = distance / maxPrediction;
	}

	Point2D targetPos = this->target->position + this->target->velocity * prediction;
	Point2D targetVelocity = targetPos - this->position;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Evade(float maxPrediction)
{
	Point2D direction = this->target->position - this->position;
	float distance = direction.Length();

	float speed = this->velocity.Length();
	float prediction;

	if (speed <= distance / maxPrediction)
	{
		prediction = maxPrediction;
	}
	else
	{
		prediction = distance / maxPrediction;
	}

	Point2D targetPos = this->target->position + this->target->velocity * prediction;
	Point2D targetVelocity = this->position - targetPos;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}

void Agent::Arrive()
{
	Point2D targetVelocity = this->target->position - this->position;

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

void Agent::Wander(float maxRotation)
{
	float change = RandomRollRange(-maxRotation, maxRotation);
	change = DegToRad(change);
	float targAngle = atan2(this->velocity.y, this->velocity.x);
	targAngle += change;

	Point2D targPoint = this->position + Point2D(cos(targAngle), sin(targAngle));
	Point2D targetVelocity = targPoint - this->position;
	targetVelocity.Normalize();

	this->velocity = targetVelocity * maxVelocity;
}