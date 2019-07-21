#include <curses.h>
#include <locale.h>
#include <random>
#include <thread>

#include "field.h"
#include "figure.h"
#include "tetris.h"

using namespace std;

Tetris::Tetris(int width, int height) 
	: m_field(new Field(width, height))
	, m_playGame(true)
	, m_activeFigure(nullptr)
	, m_speed(300)
	, m_timeSinceLastMove(0)
{ }

void Tetris::Play()
{
	TerminalSetup();

	int gameLoopSleepMs = 16;
	
	do
	{
		SpawnActiveFigureIfNeeded();

		HandleInput();

		MoveActiveFigure();

		Render();

		this_thread::sleep_for(chrono::milliseconds(gameLoopSleepMs));
		m_timeSinceLastMove += gameLoopSleepMs;
	} while (m_playGame);

	endwin();
}

void Tetris::Render()
{
	m_field->Draw();

	if(m_activeFigure != nullptr)
		m_activeFigure->Draw();

	refresh();
}

void Tetris::SpawnActiveFigureIfNeeded()
{
	if (m_activeFigure == nullptr)
	{
		std::random_device r;
		std::default_random_engine randomEngine(r());
		std::uniform_int_distribution<int> uniform_dist(0, 4);
		int figureIndex = uniform_dist(randomEngine);

		switch(figureIndex)
		{
			case 0:
				m_activeFigure = new ZFigure(m_field);
			break;
			case 1:
				m_activeFigure = new HFigure(m_field);
			break;
			case 2:
				m_activeFigure = new Square(m_field);
			break;
			case 3:
				m_activeFigure = new Line(m_field);
			break;
		}
	}
}

void Tetris::HandleInput()
{
	int input = ERR;
	do
	{	
		input = getch();
		if (input != ERR)
		{
			switch(input)
			{	case KEY_LEFT:
					m_activeFigure->MoveLeft();
					break;
				case KEY_RIGHT:
					m_activeFigure->MoveRight();
					break;
				case KEY_UP:
					m_activeFigure->RotateClockwise();
					break;
				case KEY_DOWN:
					m_activeFigure->MoveToFloor();
				break;
				case 27: /* ESC press */
					m_playGame = false;
				break;
				default:
					break;
			}
		}
	} while (input != ERR);
}

void Tetris::MoveActiveFigure()
{
	if (m_timeSinceLastMove >= m_speed)
	{
		bool moveSuccess = m_activeFigure->Move();
		if (!moveSuccess)
		{
			m_field->Merge(m_activeFigure->GetGlobalCoordinates());
			m_field->DestroyCompleteLines();
			
			delete m_activeFigure;
			m_activeFigure = nullptr;
		}
		m_timeSinceLastMove = 0;
	}
}

void Tetris::TerminalSetup()
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	curs_set(0);
}

Tetris::~Tetris()
{
	delete m_field;
}
