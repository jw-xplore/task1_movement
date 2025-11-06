#include "Agent.h"

Agent::Agent()
{
	this->position = { 100, 100};

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
	// Switch behavior
	if (Play::KeyPressed(Play::KeyboardButton::KEY_RIGHT))
	{
		int val = this->steeringType;

		if (this->steeringType < ESteeringBehavior::Wander)
			val *= 2;
		else
			val = 1;

		this->steeringType = static_cast<ESteeringBehavior>(val);
	}
	else if (Play::KeyPressed(Play::KeyboardButton::KEY_LEFT))
	{
		int val = this->steeringType;

		if (this->steeringType > ESteeringBehavior::Seek)
			val /= 2;
		else
			val = ESteeringBehavior::Wander;

		this->steeringType = static_cast<ESteeringBehavior>(val);
	}

	std::string tmp = "Type: " + std::to_string(this->steeringType);
	char const* txt = tmp.c_str();
	Play::DrawDebugText({40,40}, txt);

	// Update target position
	this->target->position = Play::GetMousePos();
	this->target->velocity = this->target->position - this->target->prevPosition;
	this->target->prevPosition = this->target->position;

	// Update agent
	//this->Steer();

	// Rotate
	if (this->velocity.x != 0 && this->velocity.y != 0)
		this->orientation = atan2(this->velocity.y, this->velocity.x);

	Entity::Update(dTime);

	// Velocity cap
	if (this->velocity.Length() > this->maxVelocity)
	{
		this->velocity.Normalize();
		this->velocity *= this->maxVelocity;
	}
}

void Agent::Draw()
{
	DrawSpriteRotated(SPRITE, this->position, 0, this->orientation, 1);
}

/*
* Steering behavior
*/

void Agent::Steer()
{
	switch (this->steeringType)
	{
	case ESteeringBehavior::Seek:
		this->Seek();
		break;
	case ESteeringBehavior::Flee:
		this->Flee();
		break;
	case ESteeringBehavior::Arrive:
		this->Arrive();
		break;
	case ESteeringBehavior::Pursue:
		this->Pursue(10);
		break;
	case ESteeringBehavior::Evade:
		this->Evade(2);
		break;
	case ESteeringBehavior::Wander:
		this->Wander(10);
		break;
	}
}

void Agent::Seek()
{
	Point2D acceleration = this->target->position - this->position;
	acceleration.Normalize();

	this->steering->linear = acceleration * maxAcceleration;
}

void Agent::Flee()
{
	Point2D acceleration = this->position - this->target->position;
	acceleration.Normalize();

	this->steering->linear = acceleration * maxAcceleration;
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
		prediction = distance / speed;
	}

	Point2D targetPos = this->target->position + this->target->velocity * prediction;
	Point2D acceleration = targetPos - this->position;
	acceleration.Normalize();

	// Visualize prediction
	Play:DrawLine(this->position, targetPos, Play::cGreen);
	Play::DrawCircle(this->target->position, 10, Play::cRed);
	Play::DrawCircle(targetPos, 10, Play::cGreen);

	this->steering->linear = acceleration * maxAcceleration;
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
	Point2D acceleration = this->position - targetPos;
	acceleration.Normalize();

	// Visualize prediction
	Play::DrawCircle(this->target->position, 10, Play::cRed);
	Play::DrawCircle(targetPos, 10, Play::cGreen);

	this->steering->linear = acceleration * maxAcceleration;
}

void Agent::Arrive()
{
	Point2D direction = this->target->position - this->position;
	float distance = direction.Length();

	// Stop
	if (distance < targetRadius)
	{
		this->velocity = { 0, 0 };
		this->steering->linear = { 0, 0 };
		return;
	}

	// Adjust velocity
	float slowRadius = 1;
	float targetSpeed;

	if (distance > slowRadius)
	{
		targetSpeed = maxVelocity;
	}
	else
	{
		targetSpeed = maxVelocity * distance / slowRadius;
	}

	Point2D targetVelocity = direction;
	targetVelocity.Normalize();
	targetVelocity *= targetSpeed;

	// Adjust acceleration
	Point2D acceleration = targetVelocity - this->velocity;
	acceleration /= timeToTarget;

	if (acceleration.Length() > maxVelocity)
	{
		acceleration.Normalize();
		acceleration = acceleration * maxVelocity;
	}

	this->steering->linear = acceleration;
}

void Agent::Wander(float maxRotation)
{
	float change = RandomRollRange(-maxRotation, maxRotation);
	change = DegToRad(change);
	float targAngle = atan2(this->velocity.y, this->velocity.x);
	targAngle += change;

	Point2D targPoint = this->position + Point2D(cos(targAngle), sin(targAngle));
	Point2D acceleration = targPoint - this->position;
	acceleration.Normalize();

	this->steering->linear = acceleration * maxAcceleration;
}

void Agent::FollowPath(Path path, int offset)
{
	Point2D nextMove = this->position + this->velocity;
	Point2D target = path.PosInPath(nextMove, offset);

	// Seek
	Point2D acceleration = target - this->position;
	acceleration.Normalize();

	this->steering->linear = acceleration * maxAcceleration;
}