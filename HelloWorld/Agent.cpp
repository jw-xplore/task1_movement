#include "Agent.h"

Agent::Agent()
{
	this->target = new SteerTarget();
	this->steeringBehavior = new SteeringBehavior();
	this->position = { 100, 100};

	// Setup target
	this->target = new SteerTarget();
	this->target->position = Play::GetMousePos();
	this->target->prevPosition = Play::GetMousePos();
}

Agent::~Agent()
{
	delete this->target;
	delete this->steering;
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
	this->Steer();

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
		this->steering->linear = steeringBehavior->seek(this->target, this->position, this->maxAcceleration);
		//this->Seek();
		break;
	case ESteeringBehavior::Flee:
		this->steering->linear = steeringBehavior->flee(this->target, this->position, this->maxAcceleration);
		//this->Flee();
		break;
	case ESteeringBehavior::Arrive:
		this->steering->linear = steeringBehavior->arrive(this->target, this->position, this->velocity, this->maxVelocity, this->maxAcceleration, 1, 0.1f);
		//this->Arrive();
		break;
	case ESteeringBehavior::Pursue:
		this->predictTarget = steeringBehavior->predictTarget(this->target, this->position, this->velocity, 10);
		this->steering->linear = steeringBehavior->seek(this->predictTarget, this->position, this->maxAcceleration);
		//delete predictTarget;
		//this->Pursue(10);
		break;
	case ESteeringBehavior::Evade:
		predictTarget = steeringBehavior->predictTarget(this->target, this->position, this->velocity, 10);
		this->steering->linear = steeringBehavior->flee(predictTarget, this->position, this->maxAcceleration);
		//delete predictTarget;
		//this->Evade(2);
		break;
	case ESteeringBehavior::Wander:
		this->Wander(10);
		break;
	}
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