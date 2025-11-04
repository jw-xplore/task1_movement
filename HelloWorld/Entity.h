/*
Custom Entity class from previous DOD course Snake project.
Handles basic game object functionality like update, drawing, translation.
Entity is supposed to be use in combination with EntityManager to handle creation, update, management (ids) and deletion.
*/

#pragma once
#include "Play.h"

using namespace Play;

class Entity
{
private:
	int id = -1;

protected:
	Point2D position = { 0,0 };
	Point2D velocity = { 0,0 };
	Point2D pivotPoint = { 0,0 };
	float rotation = 0;

public:
	Entity();
	virtual ~Entity();
	void Destroy();
	virtual void Update(float dTime);
	virtual void Draw() {}

	bool SetId(int id);
	int GetId();

	Point2D GetPosition();
	void SetPosition(Point2D position);
	void SetPosition(float x, float y);
	void Translate(Point2D position);
	void Translate(float x, float y);
	Point2D GetVelocity();
	void SetVelocity(Point2D velocity);

	const Point2D Normalized(Point2D point);
};