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
	curs_set(0);
	int gameLoopSleepMs = 16;
	int speedMs = 300;
	int timeSinceLastMove = 0;
	int input = ERR;
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
		activeFigure->Draw();
		
		do
		{	
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
		} while (input != ERR);
		
		if (timeSinceLastMove >= speedMs)
		{
			bool moveSuccess = activeFigure->Move();
			if (!moveSuccess)
			{
				field.Merge(activeFigure->GetGlobalCoordinates());
				
				delete activeFigure;
				activeFigure = nullptr;
			}
			timeSinceLastMove = 0;
		}

		refresh();
		this_thread::sleep_for(chrono::milliseconds(gameLoopSleepMs));
		timeSinceLastMove += gameLoopSleepMs;
	} while ( input != 'e');

	endwin();
}