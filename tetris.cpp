#include <iostream>
#include <vector>
#include <thread>
#include <curses.h>
#include <atomic>

using namespace std;

//    x -->
//  y 0123
//  | 1
//  | 2
// \/ 3

struct Point
{
	int X;
	int Y;
};

class Field
{
public:
	vector<vector<char>> field;

	Field(int height, int width)
	{
		field.resize(height);

		for(int i = 0; i < height; i++)
		{
			field[i] = vector<char>(width, '.');
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
				mvaddch(i, j, field[i][j]);
			}
		}
	}

	void Merge(const vector<Point>& mergePoints)
	{
		for(auto& point : mergePoints)
		{
			field[point.Y][point.X] = '#';
		}
	}

	void DestroyCompleteLines()
	{
		for(int i = 0; i < field.size(); i++)
		{
			int j = 0;
			while(j < GetWidth() && field[i][j] == '#')
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
			mvaddch(m_leftTop.Y + point.Y, m_leftTop.X + point.X, '#');
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

class InputReader
{
	private:
	thread m_thread;

	public:

		void Start()
		{
			m_thread = thread(InputReader::ReadInput, this);
		}

		static void ReadInput(void* context)
		{
			auto inputReader = static_cast<InputReader*>(context);
			
			// Make thread communication safe
			while(true)
			{
			 	this_thread::sleep_for(chrono::milliseconds(30));
			 	int c = getch();
				inputReader->NewInputAvailable.store(true);
				inputReader->LatestInput.store(c);
			}
		}

	atomic<bool> NewInputAvailable;
	atomic<int> LatestInput;
};

int main()
{
	Field field(20, 20);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	InputReader inputReader;
	inputReader.Start();

	Figure* activeFigure = nullptr;
	do
	{
		if (activeFigure == nullptr)
		{
			int a = rand() % 3;
			switch(a)
			{
				case 0:
					activeFigure = new ZFigure(&field);
				break;
				case 1:
					activeFigure = new HFigure(&field);
				break;
				case 2:
					activeFigure = new Square(&field);
				break;
			}
		}

		field.DestroyCompleteLines();
		field.Draw();
		
		if (inputReader.NewInputAvailable.load())
		{
			switch(inputReader.LatestInput.load())
			{	case KEY_LEFT:
					activeFigure->MoveLeft();
					break;
				case KEY_RIGHT:
					activeFigure->MoveRight();
					break;
				default:
					break;
			}
			inputReader.NewInputAvailable.store(false);
		}
		
		bool moveSuccess = activeFigure->Move();
		activeFigure->Draw();
		if (!moveSuccess)
		{
			field.Merge(activeFigure->GetGlobalCoordinates());
			
			delete activeFigure;
			activeFigure = nullptr;
		}

		refresh();
		this_thread::sleep_for(chrono::milliseconds(300));
	} while ( inputReader.LatestInput.load() != 'e');

	endwin();
	
    return 0;
}