#include "stdafx.h"

const double dt = 0.01;
const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 600;
bool needSort = true;
const int BALLS_COUNT = 10;
const int MAX_VELOCITY = 5000;

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
		pos.x += vel.x * dt;
		pos.y += vel.y * dt;

		if (pos.x - radius < WINDOW_SIZE_X/5)
		{
			pos.x = radius + WINDOW_SIZE_X/5;
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
			sprintf(buff, "Score: %d, id: %d.", arr[i].score, arr[i].id);
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
		Sort();
		Draw();
	}
};

int main()
{
	txCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);

	srand(time(0));

	Ball balls[BALLS_COUNT];

	ScoreBoard Board;

	txSelectFont("Calibri Light", 30);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		Board.Refresh(balls, BALLS_COUNT);
		for (int i = 0; i < BALLS_COUNT; i++)
			balls[i].Run();
		Board.Run();

		txSetColor(TX_BLACK);
	
		txSetFillColor(TX_WHITE);
		txSleep(20);
		txClear();
		txClearConsole();
	}

    return 0;
}
