#pragma once
#include <vector>
#include "Play.h"

class Path
{
private:
	std::vector<Play::Point2D> points;

public:
	Path(std::vector<Play::Point2D> path);
	void DrawPath();
	Play::Point2D PosInPath(Play::Point2D position, int offset);
};