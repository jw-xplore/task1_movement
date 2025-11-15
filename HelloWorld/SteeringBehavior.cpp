#include "SteeringBehavior.h"


Point2D SteeringBehavior::seek(SteerTarget* target, Point2D pos, float maxAccel)
{
	Point2D acceleration = target->position - pos;
	acceleration.Normalize();

	return acceleration * maxAccel;
}

Point2D SteeringBehavior::flee(SteerTarget* target, Point2D pos, float maxAccel)
{
	Point2D acceleration = pos - target->position;
	acceleration.Normalize();

	return acceleration * maxAccel;
}

Point2D SteeringBehavior::arrive(SteerTarget* target, Point2D pos, Point2D vel, float maxVel, float maxAccel, float radius, float timeToTarget)
{
	Point2D direction = target->position - pos;
	float distance = direction.Length();

	// Stop
	if (distance < radius)
	{
		return Point2D{ 0, 0 };
	}

	// Adjust velocity
	float slowRadius = 100;
	float targetSpeed;

	if (distance > slowRadius)
	{
		targetSpeed = maxVel;
	}
	else
	{
		targetSpeed = maxVel * distance / slowRadius;
	}

	Point2D targetVelocity = direction;
	targetVelocity.Normalize();
	targetVelocity *= targetSpeed;

	// Adjust acceleration
	Point2D acceleration = targetVelocity - vel;
	acceleration /= timeToTarget;

	if (acceleration.Length() > maxAccel)
	{
		acceleration.Normalize();
		acceleration = acceleration * maxAccel;
	}

	Play::DrawLine(pos, pos + acceleration, cOrange);
	return acceleration;
}

SteerTarget* SteeringBehavior::predictTarget(SteerTarget* target, Point2D pos, Point2D vel, float maxPredict)
{
	Point2D direction = target->position - pos;
	float distance = direction.Length();

	float speed = vel.Length();
	float prediction;

	if (speed <= distance / maxPredict)
	{
		prediction = maxPredict;
	}
	else
	{
		prediction = distance / maxPredict;
	}

	SteerTarget* newTarget = new SteerTarget();
	newTarget->position = target->position + target->velocity * prediction;

	// Debug
	Play::DrawCircle(target->position, 10, Play::cRed);
	Play::DrawCircle(newTarget->position, 10, Play::cGreen);

	return newTarget;
}

SteerTarget* SteeringBehavior::followPath(Path* path, Point2D pos, Point2D vel, int offset)
{
	Point2D nextMove = pos + vel;
	Point2D target = path->PosInPath(nextMove, offset);

	SteerTarget* newTarget = new SteerTarget();
	newTarget->position = path->PosInPath(nextMove, offset);

	return newTarget;

}

Point2D SteeringBehavior::separate(std::vector<Entity*> obstacles, Entity* self, Point2D pos, float threshold, float maxAccel)
{
	Point2D acceleration = { 0, 0};
	Point2D dir;
	float dist;

	for (int i = 0; i < obstacles.size(); i++)
	{
		if (obstacles[i] == self)
			continue;

		dir = obstacles[i]->position - pos;
		dist = dir.Length();

		if (dist < threshold)
		{
			float strenght = maxAccel * (threshold - dist) / threshold;
			dir.Normalize();
			acceleration = -dir * strenght;
		}
	}

	Play::DrawLine(pos, pos + acceleration, cOrange);

	return acceleration;
}

Point2D SteeringBehavior::avoidCollisions(std::vector<Entity*> obstacles, Entity* self, float radius, float maxAccel)
{
	// Find closest target
	float shortestTime = 99999;

	Entity* firstTarget = nullptr;
	float firstMinSeparation;
	float firstDistance;
	Point2D firstRelativePos;
	Point2D firstRelativeVel;


	for (int i = 0; i < obstacles.size(); i++)
	{
		if (obstacles[i] == self)
			continue;

		// Time to collision
		Point2D relativePos = obstacles[i]->position - self->position;
		Point2D relativeVel = obstacles[i]->velocity - self->velocity;
		float relativeSpeed = relativeVel.Length();

		float timeToCollide = relativePos.Dot(relativeVel) / (relativeSpeed * relativeSpeed);

		// Check if there will be collision
		float distance = relativePos.Length();
		float minSeparation = distance - relativeSpeed * timeToCollide;

		if (minSeparation > radius * 2)
		{
			continue;
		}

		// Check the shortest
		if (timeToCollide > 0 && timeToCollide < shortestTime)
		{
			shortestTime = timeToCollide;
			firstTarget = obstacles[i];
			firstMinSeparation = minSeparation;
			firstDistance = distance;
			firstRelativePos = relativePos;
			firstRelativeVel = relativeVel;
		}
	}

	// Calculate steering
	if (firstTarget == nullptr)
		return { 0, 0 };

	Point2D relativePos;

	if (firstMinSeparation <= 0 || firstDistance < radius * 2)
	{
		// Apply steering on ongoing collision
		relativePos = firstTarget->position - self->position;
	}
	else
	{
		relativePos = firstRelativePos + firstRelativeVel * shortestTime;
	}

	relativePos.Normalize();
	Play::DrawLine(self->position, self->position + relativePos * maxAccel, cOrange);
	return relativePos * maxAccel;
}

SteerTarget* SteeringBehavior::avoidObstacles(CollisionHandler* collisiions, std::vector<Path*> walls, Entity* self, float avoidDist, float lookahead)
{
	SteerTarget* newTarget = new SteerTarget();
	newTarget->position = { 0, 0 };

	Point2D ray = self->velocity;
	if (ray.x == 0 && ray.y == 0)
		return newTarget;

	ray.Normalize();
	ray *= lookahead;

	Point2D posRay = self->position + ray;

	Collision col = collisiions->detectCollision(walls, posRay);
	float distance = (col.position - posRay).Length();

	if (distance <= avoidDist)
	{
		newTarget->position += col.normal * avoidDist;
		DrawLine(self->position, newTarget->position, cGreen);
	}

	DrawLine(self->position, self->position + ray, cOrange);
	DrawCircle(self->position + ray, avoidDist, cYellow);
	DrawCircle(col.position, 10, cOrange);
	DrawCircle(self->position + newTarget->position, 10, cGreen);

	return newTarget;
}
