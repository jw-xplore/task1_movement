#include "CoordinatedMovement.h"

Play::Point2D CoordinatedMovement::memberPosition(Entity* member)
{
	Play::Point2D position = { 0, 0 };

	for (int i = 0; i < members.size(); i++)
	{
		if (member == members[i])
		{
			float ang = atan2(leader->velocity.y, leader->velocity.x);
			int distFactor = 1 + i / 2;

			if (i % 2 == 0)
				ang += Play::DegToRad(90 + angle);
			else
				ang -= Play::DegToRad(90 + angle);

			position = { cos(ang), sin(ang) };
			position *= distFactor * distance;
			position += leader->position;
			break;
		}
	}

	Play::DrawCircle(position, 20, cMagenta);
	return position;
}