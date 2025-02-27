#include "Stage.h"
#include "./globals.h"
#include <cstdlib>
#include <ctime>

Stage::Stage()
	:goalRect(0, 0, 0, 0)
{
	stageData = vector(STAGE_HEIGHT, vector<STAGE_OBJ>(STAGE_WIDTH, STAGE_OBJ::WALL));

	srand(time(0));

	GenerateMaze();

	GenerateGoal();

	setStageRects();
}

Stage::~Stage()
{
}

void Stage::GenerateGoal()
{
	int goalX, goalY;

	do
	{
		goalX = rand() % (STAGE_WIDTH - 2) + 1;
		goalY = rand() % (STAGE_HEIGHT - 2) + 1;
	} while (stageData[goalY][goalX] == STAGE_OBJ::WALL);

	stageData[goalY][goalX] = STAGE_OBJ::GOAL;

	//goalRect=Rect(goalX * CHA_WIDTH, goalY* CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT);
}

void Stage::GenerateMaze()
{
	int startX = rand() % (STAGE_WIDTH / 2) * 2 + 1;
	int startY = rand() % (STAGE_HEIGHT / 2) * 2 + 1;

	CarvePath(startX, startY);
}

void Stage::CarvePath(int x, int y)
{
	stageData[y][x] = STAGE_OBJ::EMPTY;

	const int directions[4][2] = { { 0,1 }, {1,0 },{0,-1},{-1,0} };
	int dirs[4] = { 0,1,2,3 };

	for (int i = 0; i < 4; i++)
	{
		int j = rand() % 4;
		std::swap(dirs[i], dirs[j]);
	}

	for (int i = 0; i < 4; i++) {
		int nx = x + directions[dirs[i]][0] * 2;
		int ny = y + directions[dirs[i]][1] * 2;

		if (IsValidMove(nx, ny)) {
			stageData[ny][nx] = STAGE_OBJ::EMPTY;

			stageData
				[y + directions[dirs[i]][1]]
				[x + directions[dirs[i]][0]] = STAGE_OBJ::EMPTY;

			CarvePath(nx, ny);
		}
	}
}

bool Stage::IsValidMove(int x, int y)
{
	return x > 0 && x < STAGE_WIDTH - 1 && y>0 && y < STAGE_HEIGHT - 1 && stageData[y][x] == STAGE_OBJ::WALL;
}


void Stage::Update()
{
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			switch (stageData[y][x])
			{
			case STAGE_OBJ::EMPTY:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(102, 205, 170), TRUE);
				break;
			case STAGE_OBJ::WALL:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(119, 136, 153), TRUE);
				break;
			case STAGE_OBJ::GOAL:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(255, 215, 0), TRUE);
				break;
			default:
				break;
			}
		}
	}
}

void Stage::setStageRects()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x] == STAGE_OBJ::WALL)
			{
				stageRects.push_back(Rect(x * CHA_WIDTH, y * CHA_HEIGHT,  CHA_WIDTH, CHA_HEIGHT));
			}
			else if (stageData[y][x] == STAGE_OBJ::GOAL)
			{
				goalRect = Rect(x * CHA_WIDTH, y * CHA_HEIGHT, CHA_WIDTH, CHA_HEIGHT);
			}
		}
	}

}