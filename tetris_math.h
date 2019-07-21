#pragma once

struct Point
{
	int X;
	int Y;
};

class TetrisMath
{
public:

	static void Rotate90DegsClockwiseAndOffsetUp(Point* point, int offsetY)
	{
		std::swap(point->X, point->Y);
		point->Y = -point->Y + offsetY;
	}

	static Point Rotate90DegsClockwiseAndOffsetUp(const Point& point, int offsetY)
	{
		Point copy(point);

		Rotate90DegsClockwiseAndOffsetUp(&copy,  offsetY);
		
		return copy;
	}
};
