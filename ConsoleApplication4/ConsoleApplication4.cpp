#include "stdafx.h"

const double DT = 0.01;
const int    WINDOW_SIZE_X = 800;
const int    WINDOW_SIZE_Y = 600;
      bool   needSort      = true;
const int    BALLS_COUNT   = 10;
const int    MAX_VELOCITY  = 500;
const bool   FULLSCREEN    = true;
const int    FONT_SIZE     = 25;

struct DPOINT
{
	double x;
	double y;
};

class Ball
{
public:
	DPOINT pos = { 0, 0 };
	DPOINT vel = { 0, 0 };
	double radius = 10;

	int score = 0;

	COLORREF color = TX_BLACK;

	int id = 0;

	void Physics()
	{
		pos.x += vel.x * DT;
		pos.y += vel.y * DT;

		if (pos.x - radius < WINDOW_SIZE_X / 5 + WINDOW_SIZE_X / 192)
		{
			pos.x = radius + WINDOW_SIZE_X / 5 + WINDOW_SIZE_X / 192;
			vel.x = -vel.x;
			score++;
			needSort = true;
		}

		if (pos.y - 10 < 0)
		{
			pos.y = radius;
			vel.y = -vel.y;
			score++;
			needSort = true;
		}

		if (pos.x + radius > WINDOW_SIZE_X)
		{
			pos.x = WINDOW_SIZE_X - radius;
			vel.x = -vel.x;
			score++;
			needSort = true;
		}

		if (pos.y + radius > WINDOW_SIZE_Y)
		{
			pos.y = WINDOW_SIZE_Y - radius;
			vel.y = -vel.y;
			score++;
			needSort = true;
		}
	}

	void Draw()
	{
		txSetColor(color);
		txSetFillColor(color);

		txCircle(pos.x, pos.y, radius);
	}

	Ball()
	{
		vel.x = rand() % MAX_VELOCITY / 2 - MAX_VELOCITY;
		vel.y = rand() % MAX_VELOCITY / 2 - MAX_VELOCITY;

		pos.x = rand() % WINDOW_SIZE_X;
		pos.y = rand() % WINDOW_SIZE_Y;

		color = RGB(rand() % 255, rand() % 255, rand() % 255);

		Run();
		id = rand()%100;
	}

	void Run()
	{
		Physics();
		Draw();
	}
};

struct SCORECELL
{
	DPOINT pos;
	int id;
	int score;
	COLORREF color;
};

class ScoreBoard
{
public:
	SCORECELL arr[BALLS_COUNT] = {};

	void Refresh(Ball ball[], int size)
	{
		for (int i = 0; i < BALLS_COUNT; i++)
		{
			arr[i].id = ball[i].id;
			arr[i].score = ball[i].score;
			arr[i].pos = ball[i].pos;
			arr[i].color = ball[i].color;
		}

		Sort();
	}

	void Draw()
	{
		char buff[128] = "";

		for (int i = 0; i < 10; i++)
		{
			txSetColor(arr[i].color);
			txSetFillColor(arr[i].color);
			txRectangle(0, WINDOW_SIZE_Y / 10 * i, WINDOW_SIZE_X/5, WINDOW_SIZE_Y/10 * i + WINDOW_SIZE_Y / 10);
			txSetColor(TX_WHITE);
			sprintf(buff, "#%d    Score: %d,   id: %d.", i + 1, arr[i].score, arr[i].id);
			txDrawText(0, i * WINDOW_SIZE_Y / 10, WINDOW_SIZE_X/5, i * WINDOW_SIZE_Y / 10 + WINDOW_SIZE_Y / 10, buff);
		}
	}

	void swap(SCORECELL balls[], int a, int b)
	{
		SCORECELL buff = balls[a];
		balls[a] = balls[b];
		balls[b] = buff;
	}

	void Sort()
	{
		bool sorted = false;

		while (true)
		{
			sorted = false;

			for (int i = 0; i < BALLS_COUNT - 1; i++)
			{
				if (arr[i].score < arr[i + 1].score)
				{
					swap(arr, i, i + 1);
					sorted = true;
				}
			}

			if (sorted == false) break;
		}
	}

	void Run()
	{

		Draw();
	}
};

int main()
{
	txCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	txDisableAutoPause();
	srand(time(0));
	Ball balls[BALLS_COUNT];
	ScoreBoard Board;
	txSelectFont("Calibri Light", FONT_SIZE);
	int counter = 0;

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		if (counter % 10 == 0) Board.Refresh(balls, BALLS_COUNT);
		for (int i = 0; i < BALLS_COUNT; i++)
			balls[i].Run();
		Board.Run();

		txSetColor(TX_ORANGE);
		txSetFillColor(TX_ORANGE);
		txRectangle(WINDOW_SIZE_X / 5, 0, WINDOW_SIZE_X / 5 + WINDOW_SIZE_X / 192, WINDOW_SIZE_Y);

		txSetFillColor(TX_BLACK);
		txSleep(20);
		txClear();
		txClearConsole();

		counter++;
	}

    return 0;
}
