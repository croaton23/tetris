#pragma once

#include "tetris_math.h"

class Figure
{
	public:

	Figure(Field* field): m_field(field), m_leftTop{field->GetWidth()  / 2, 0}
	{ }

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

	void Draw() const
	{
		for(auto& point : m_localCoordinates)
		{
			char a = '█';
			mvaddch(m_leftTop.Y + point.Y, m_leftTop.X + point.X, a);
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
};


class Square : public Figure
{
	public:
	Square(Field* field) : Figure(field) 
	{
		m_localCoordinates.clear();
		m_localCoordinates.push_back({0,0});
		m_localCoordinates.push_back({0,1});
		m_localCoordinates.push_back({1,0});
		m_localCoordinates.push_back({1,1});

		m_leftTop.X = field->GetWidth()  / 2;
		m_leftTop.Y = 0;
	}
};

class HFigure : public Figure
{
	public:
	HFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.clear();
		m_localCoordinates.push_back({0,0});
		m_localCoordinates.push_back({0,1});
		m_localCoordinates.push_back({1,1});
		m_localCoordinates.push_back({1,2});

		m_leftTop.X = field->GetWidth()  / 2;
		m_leftTop.Y = 0;
	}
};

class ZFigure : public Figure
{
	public:
	ZFigure(Field* field) : Figure(field) 
	{
		m_localCoordinates.clear();
		m_localCoordinates.push_back({0,0});
		m_localCoordinates.push_back({1,0});
		m_localCoordinates.push_back({1,1});
		m_localCoordinates.push_back({2,1});
	}
};
