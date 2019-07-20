#include <curses.h>
#include <thread>

#include "field.h"
#include "figure.h"
#include "tetris.h"

using namespace std;

void Tetris::Play()
{
	Field field(20, 20);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);

	int input = 0;
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
		
		input = getch();
		if (input != ERR)
		{
			switch(input)
			{	case KEY_LEFT:
					activeFigure->MoveLeft();
					break;
				case KEY_RIGHT:
					activeFigure->MoveRight();
					break;
				default:
					break;
			}
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
	} while ( input != 'e');

	endwin();
}