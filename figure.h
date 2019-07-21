#pragma once

#include <wchar.h>
#include "tetris_math.h"

class Figure
{
	public:

	Figure(Field* field): m_field(field), m_leftTop{field->GetWidth()  / 2, 0}
	{ 
		m_localCoordinates.clear();
	}

	bool Move()
	{
		for(auto& point : m_localCoordinates)
		{
			Point globalPoint { m_leftTop.X + point.X, m_leftTop.Y + point.Y };

			if (globalPoint.Y >= m_field->GetHeight() - 1)
				return false;

			if (m_field->field[globalPoint.Y + 1][globalPoint.X] != '.')
				return false;
		}

		m_leftTop.Y++;

		return true;
	}

	void MoveLeft()
	{
		for(auto& point : m_localCoordinates)
		{
			Point globalPoint { m_leftTop.X + point.X, m_leftTop.Y + point.Y };

			if (globalPoint.X == 0)
				return;

			if (m_field->field[globalPoint.Y][globalPoint.X - 1] != '.')
				return;
		}

		m_leftTop.X--;
	}
	
	void MoveRight()
	{
		for(auto& point : m_localCoordinates)
		{
			Point globalPoint { m_leftTop.X + point.X, m_leftTop.Y + point.Y };

			if (globalPoint.X + 1 == m_field->GetWidth())
				return;

			if (m_field->field[globalPoint.Y][globalPoint.X + 1] != '.')
				return;
		}

		m_leftTop.X++;
	}

	void RotateClockwise()
	{
		for(auto& point : m_localCoordinates)
		{
			Point rotatedPoint = TetrisMath::Rotate90DegsClockwiseAndOffsetUp(point, m_width);
			if (m_leftTop.X + rotatedPoint.X >= m_field->GetWidth() 
				|| m_leftTop.Y + rotatedPoint.Y >= m_field->GetHeight()
				|| m_field->field[m_leftTop.Y + rotatedPoint.Y][m_leftTop.X + rotatedPoint.X] != '.')
			{
				return;
			}
		}

		for(auto& point : m_localCoordinates)
		{
			TetrisMath::Rotate90DegsClockwiseAndOffsetUp(&point, m_width);
		}

		std::swap(m_height, m_width);
	}

	void MoveToFloor()
	{
		while(Move());
	}

	void Draw() const
	{
		for(auto& point : m_localCoordinates)
		{
    		mvaddwstr(m_leftTop.Y + point.Y, m_leftTop.X + point.X, L"█");
		}
	}

	vector<Point> GetGlobalCoordinates() const
	{
		vector<Point> result;
		for(auto& point : m_localCoordinates)
		{
			result.push_back({m_leftTop.X + point.X, m_leftTop.Y + point.Y});
		}

		return result;
	}

protected:
	Point m_leftTop;
	vector<Point> m_localCoordinates;
	Field* m_field;
	int m_width;
	int m_height;
};


class Square : public Figure
{
	public:
	Square(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {0,0}, {0,1}, {1,0}, {1,1} } );
		m_width = 2;
		m_height = 2;
	}
};

class HFigure : public Figure
{
	public:
	HFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {0,0}, {0,1}, {1,1}, {1,2} } );
		m_width = 2;
		m_height = 3;
	}
};

class ZFigure : public Figure
{
	public:
	ZFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {0,0}, {1,0}, {1,1}, {2,1} } );
		m_width = 3;
		m_height = 2;
	}
};

class JFigure : public Figure
{
	public:
	JFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {1,0}, {1,1}, {1,2}, {0,2} } );
		m_width = 2;
		m_height = 3;
	}
};

class LFigure : public Figure
{
	public:
	LFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {0,0}, {0,1}, {0,2}, {1,2} } );
		m_width = 2;
		m_height = 3;
	}
};

class Line : public Figure
{
	public:
	Line(Field* field) : Figure(field) 
	{
		m_localCoordinates.insert( m_localCoordinates.end(), { {0,0}, {1,0}, {2,0}, {3,0} } );
		m_width = 4;
		m_height = 1;
	}
};
