#define _X_OPEN_SOURCE_EXTENDED
#define NCURSES_WIDECHAR 1
#include <curses.h>
#include <locale.h>
#include <thread>

#include "field.h"
#include "figure.h"
#include "tetris.h"

using namespace std;

void Tetris::Play()
{
	Field field(20, 20);
	
	setlocale(LC_ALL, "");
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
		field.DestroyCompleteLines();

		if (activeFigure == nullptr)
		{
			int a = rand() % 4;
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
				case 3:
					activeFigure = new Line(&field);
				break;
			}
		}

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
					case KEY_UP:
						activeFigure->RotateClockwise();
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

		field.Draw();
		if(activeFigure)
			activeFigure->Draw();
		
		refresh();
		this_thread::sleep_for(chrono::milliseconds(gameLoopSleepMs));
		timeSinceLastMove += gameLoopSleepMs;
	} while ( input != 'e');

	endwin();
}