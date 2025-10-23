#pragma once
#include "Cell.h"



struct Grid
{
	Cell** cells;
	int Column;
	int Line;
	int TotalMine;
};

int AskDiff();

void* SafeAlloc(int byteCount);

void DisplayGrid(Grid grid, int diff);

void ChooseCase(Grid* grid, int* col, int* line);

void PlaceMine(Grid* grid, int FirstX, int FirstY);

Grid InitGrid(int diff);

int CountAdjacentMine(Grid grid);

void RevealCase(Grid* grid, int x, int y);

bool CheckVictory(Grid* grid);

void RevealAllMines(Grid* grid);

void ToggleFlag(Grid* grid, int x, int y);

void PlayerAction(Grid* grid, int x, int y, int difficulty, bool* gameOver);
