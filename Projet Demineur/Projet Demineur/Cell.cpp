#include "Cell.h"
#include "Grid.h"
#include <iostream>
using namespace std;

Cell InitCell(Cell cell)
{
    cell.GotMine = false;
    cell.Reveal = false;
    cell.MarkedByFlag = false;
    cell.AdjacentMine = 0;

    return cell;
}

char GetCell(Cell cell)
{
    if (cell.MarkedByFlag)
        return 'F';
    else if (!cell.Reveal)
        return '.';
    else if (cell.GotMine)
        return '*';
    else if (cell.AdjacentMine > 0)
        return '0' + cell.AdjacentMine;
    else
        return ' ';

}

void CellState(Cell cell)
{
    cout << "Mine: " << cell.GotMine
        << ", Reveal ? " << cell.Reveal
        << ", Flag ? " << cell.MarkedByFlag
        << ", Adjacent: " << cell.AdjacentMine << endl;
}

