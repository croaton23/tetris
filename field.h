#pragma once

#include <vector>
#include <curses.h>
#include "tetris_math.h"

using namespace std;

//    x -->
//  y 0123
//  | 1
//  | 2
// \/ 3

class Field
{
public:
	vector<vector<wchar_t>> field;

	Field(int height, int width)
	{
		field.resize(height);

		for(int i = 0; i < height; i++)
		{
			field[i] = vector<wchar_t>(width, '.');
		}
	}

	int GetWidth()
	{
		return field.size() > 0 ? field[0].size() : 0;
	}

	int GetHeight()
	{
		return field.size();
	}

	void Draw()
	{
		for(int i = 0; i < field.size(); i++)
		{
			for(int j = 0; j < field[0].size(); j++)
			{
    			mvaddwstr(i, j, &field[i][j]);
			}
		}
	}

	void Merge(const vector<Point>& mergePoints)
	{
		for(auto& point : mergePoints)
		{
			field[point.Y][point.X] = L'█';
		}
	}

	void DestroyCompleteLines()
	{
		for(int i = 0; i < field.size(); i++)
		{
			int j = 0;
			while(j < GetWidth() && field[i][j] == L'█')
				j++;
			
			if (j == GetWidth())
			{
				for(int k = 0; k < GetWidth(); k++)
					field[i][k] = '.';

				MoveDownAfterDestroyedLine(i);
			}
		}
	}

private:
	void MoveDownAfterDestroyedLine(int line)
	{
		for(int i = line; i > 0; i--)
			for(int j = 0; j < GetWidth(); j++)
				field[i][j] = field[i-1][j];
		
		for(int j = 0; j < GetWidth(); j++)
			field[0][j] = '.';
	}
};