#pragma once

class Field;
class Figure;

class Tetris
{
public:
    Tetris(int width, int height);
	void Play();
    ~Tetris();

private:
    void TerminalSetup();
    void SpawnActiveFigureIfNeeded();
    void HandleInput();
    void MoveActiveFigure();
    void Render();
	Field* m_field;
	Figure* m_activeFigure;
	bool m_playGame;
    int m_speed;
    int m_timeSinceLastMove;
};