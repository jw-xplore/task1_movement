#include "Path.h"

Path::Path(std::vector<Play::Point2D> path)
{
	this->points = path;
}

void Path::DrawPath()
{
	Play::DrawCircle(this->points[0], 10, Play::cRed);

	if (this->points.size() < 2)
	{
		return;
	}

	int last = this->points.size() - 1;
		
	for (int i = 0; i < last; i++)
	{
		Play::DrawLine(this->points[i], this->points[i + 1], Play::cWhite);
	}

	Play::DrawCircle(this->points[last], 10, Play::cGreen);
}

Play::Point2D Path::PosInPath(Play::Point2D position, int offset)
{
	int closest = 0;
	int last = this->points.size() - 1;
	float distance = (position - this->points[0]).Length();

	// Find closest
	for (int i = 1; i < last; i++)
	{
		float newDist = (position - this->points[i]).Length();
		if (distance > newDist)
		{
			distance = newDist;
			closest = i;
		}
	}

	// Point projection
	Play::Point2D targetDist = position - this->points[closest];
	Play::Point2D pathSegment = this->points[closest + 1] - this->points[closest];

	Play::Point2D targetDistNorm = targetDist;
	Play::Point2D pathSegmentNorm = pathSegment;
	targetDistNorm.Normalize();
	pathSegmentNorm.Normalize();

	float projectionL = targetDist.x * pathSegmentNorm.x + targetDist.y * pathSegmentNorm.y;

	float dot = targetDist.x * pathSegment.x + targetDist.y * pathSegment.y;
	float angle = acos((dot) / (targetDistNorm.Length() * pathSegmentNorm.Length()));
	//float angle = targetDist.x * pathSegment.x + targetDist.y * pathSegment.y;
	//angle /= targetDist.Length();

	Play::Point2D pos = this->points[closest] + pathSegmentNorm * (projectionL + offset);

	Play::DrawLine(this->points[closest], pos, Play::cBlue);
	Play::DrawLine(this->points[closest], position, Play::cRed);
	Play::DrawCircle(pos, 10, Play::cYellow);
	return pos;
}