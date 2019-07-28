#pragma once

#include <vector>
#include <curses.h>
#include "tetris_math.h"

using namespace std;

//    x ==>
//  y 0123
// || 1
// || 2
// \/ 3

class Field
{
public:

	Field(int height, int width)
	{
		m_field.resize(height);

		for(int i = 0; i < height; i++)
		{
			m_field[i] = vector<wchar_t>(width, L'.');
		}
	}

	void SetEmpty(int x, int y)
	{
		m_field[x][y] = L'.';
	}

	void SetOccupied(int x, int y)
	{
		m_field[x][y] = L'█';
	} 

	bool GetIsEmpty(int x, int y)
	{
		return m_field[x][y] == L'.';
	}

	int GetWidth()
	{
		return m_field.size() > 0 ? m_field[0].size() : 0;
	}

	int GetHeight()
	{
		return m_field.size();
	}

	void Draw()
	{
		for(int i = 0; i < m_field.size(); i++)
		{
			for(int j = 0; j < m_field[0].size(); j++)
			{
    			mvaddwstr(i, j, &m_field[i][j]);
			}
		}
	}

	void Merge(const vector<Point>& mergePoints)
	{
		for(auto& point : mergePoints)
		{
			m_field[point.Y][point.X] = L'█';
		}
	}

	void DestroyCompleteLines()
	{
		for(int i = 0; i < m_field.size(); i++)
		{
			int j = 0;
			while(j < GetWidth() && m_field[i][j] == L'█')
				j++;
			
			if (j == GetWidth())
			{
				for(int k = 0; k < GetWidth(); k++)
					m_field[i][k] = L'.';

				MoveDownAfterDestroyedLine(i);
			}
		}
	}

private:
	vector<vector<wchar_t>> m_field;

	void MoveDownAfterDestroyedLine(int line)
	{
		for(int i = line; i > 0; i--)
			for(int j = 0; j < GetWidth(); j++)
				m_field[i][j] = m_field[i-1][j];
		
		for(int j = 0; j < GetWidth(); j++)
			m_field[0][j] = L'.';
	}
};