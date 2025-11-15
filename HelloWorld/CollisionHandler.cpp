#include "CollisionHandler.h"

Collision CollisionHandler::detectCollision(std::vector<Path*> walls, Play::Point2D pos)
{
	Play::Point2D closestPos;
	int closestPath = 0;
	float closestPathDist = -1;

	for (int i = 0; i < walls.size(); i++)
	{
		Play::Point2D closestPoint = walls[i]->PosInPath(pos, 0);
		float dist = (closestPoint - pos).Length();

		if (closestPathDist > dist)
		{
			closestPos = closestPoint;
			closestPathDist = dist;
			closestPath = i;
		}
	}

	Play::Point2D touchPos = walls[closestPath]->NormalInPath(pos);
	Play::Point2D normal = pos - touchPos;
	normal.Normalize();

	Collision col;
	col.position = touchPos;
	col.normal = normal;

	return col;
}