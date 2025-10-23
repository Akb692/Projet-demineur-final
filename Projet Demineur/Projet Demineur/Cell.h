#pragma once



struct Cell
{
	bool GotMine;
	bool Reveal;
	bool MarkedByFlag;
	int AdjacentMine;
};

Cell InitCell(Cell cell);

char GetCell(Cell cell);

void CellState(Cell cell);

