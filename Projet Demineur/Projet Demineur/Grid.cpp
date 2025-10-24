#include "Grid.h"
#include "Cell.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int AskDiff()
{
    int diff;

    do
    {
        cout << "Choose a difficulty : 1 ( 9 * 9 )" << endl;
        cout << "		      2 ( 16 * 16 )" << endl;
        cout << "		      3 ( 30 * 16 ) " << endl;
        cin >> diff;

    } while (diff > 1 and diff > 3);

    cout << endl;
    return diff;
}

void* SafeAlloc(int byteCount)
{
    void* p = malloc(byteCount);
    if (p == nullptr)
        exit(1);
    return p;
}

void DisplayGrid(Grid grid, int diff)
{
    system("cls");

    int rows = 0;
    int cols = 0;

    switch (diff)
    {
    case 1:
        rows = 9; cols = 9;
        break;
    case 2:
        rows = 16; cols = 16;
        break;
    case 3:
        rows = 30; cols = 16;
        break;
    default:
        cout << "Difficulte inconnue" << endl;
        return;
    }

    cout << "     ";
    for (int k = 1; k <= cols; k++) {
        if (k < 10)
            cout << "  " << k << " ";
        else
            cout << " " << k << " ";
    }
    cout << endl;

    cout << "     ";
    for (int j = 0; j < cols; j++) {
        cout << "+---";
    }
    cout << "+" << endl;

    for (int i = 0; i < rows; i++) {
        if (i + 1 < 10)
            cout << "  " << i + 1 << "  |";
        else
            cout << " " << i + 1 << "  |";

        for (int j = 0; j < cols; j++) {
            cout << GetCell(grid.cells[i][j]) << "  |";
        }

        cout << endl;

        cout << "     ";
        for (int j = 0; j < cols; j++) {
            cout << "+---";
        }
        cout << "+" << endl;
    }
}


void ChooseCase(Grid* grid, int* col, int* line)
{
    cout << "Choose a column and a line to reveal a case" << endl;
    cin >> *col;
    cin >> *line;

    while (*col < 0 or *col > grid->Column or *line < 0 or *line > grid->Line)
    {
        cout << "invalid coordinates" << endl;
        cout << "Choose a column and a line to reveal a case" << endl;
        cin >> *col;
        cin >> *line;
    }
}

void PlaceMine(Grid* grid, int FirstX, int FirstY)
{
    for (int i = 0; i < grid->TotalMine; i++)
    {
        int x = rand() % grid->Line;  // we randomly choose a case
        int y = rand() % grid->Column;

        if (!grid->cells[x][y].GotMine and x != FirstX and y != FirstY) // we don't put the mine on the first player choice
        {
            grid->cells[x][y].GotMine = true;
        }
    }
}

Grid InitGrid(int diff)
{
    Grid grid;

    switch (diff) // first we get the column and line
    {
    case 1:
    {
        grid.Column = grid.Line = 9; 
    }break;

    case 2:
    {
        grid.Column = grid.Line = 17;
    }break;

    case 3:
    {
        grid.Column = 17;
        grid.Line = 30;
    }break;


    default:
    {
        grid.cells = nullptr;
    }break;

    }

    grid.TotalMine = grid.Column * grid.Line * 0.15; // we can calculate the number of mine in the grid

    grid.cells = (Cell**)SafeAlloc(sizeof(Cell*) * grid.Line); // and then we can build the grid in the memory adress
    for (int i = 0; i < grid.Line; ++i)
    {
        grid.cells[i] = (Cell*)SafeAlloc(sizeof(Cell) * grid.Column);
    }

    for (int i = 0; i < grid.Line; i++)

    {
        for (int j = 0; j < grid.Column; j++)
        {
            grid.cells[i][j] = InitCell(grid.cells[i][j]); // and finally we initiate all the cells
        }
    }

    return grid;
}

int CountAdjacentMine(Grid grid)
{
    int LineX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 }; // coordinates of all the adjacents mines
    int ColumnY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < grid.Line; i++)
    {
        for (int j = 0; j < grid.Column; j++)
        {
            if (grid.cells[i][j].GotMine == false) // we only count case that dont have a mine
            {
                int count = 0;

                for (int k = 0; k < 8; k++)
                {
                    int ni = i + LineX[k];
                    int nj = j + ColumnY[k];


                    if (ni >= 0 and ni < grid.Line and nj >= 0 and nj < grid.Column)
                    {
                        if (grid.cells[ni][nj].GotMine) // if an adjacent case got a mine we increase count
                        {
                            count++;
                        }
                    }
                }


                grid.cells[i][j].AdjacentMine = count;
            }
        }
    }

    return 0;
}

void RevealCase(Grid* grid, int x, int y)
{

    if (x < 0 or x >= grid->Line or y < 0 or y >= grid->Column) // the case has to be in bounds
        return;

    Cell cell = grid->cells[x][y];


    if (cell.Reveal or cell.MarkedByFlag) // if its already revealed or it got a flag we do nothing
        return;

    grid->cells[x][y].Reveal = true;

    if (cell.GotMine) // if it's a mine we stop
        return;


    if (cell.AdjacentMine == 0) // if the case dont have any adjacent mine it reveal them by recursion 
    {
        int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        for (int i = 0; i < 8; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            RevealCase(grid, nx, ny);
        }
    }
}

bool CheckVictory(Grid* grid)
{
    for (int i = 0; i < grid->Line; i++)
    {
        for (int j = 0; j < grid->Column; j++)
        {
            if (grid->cells[i][j].GotMine == false and grid->cells[i][j].Reveal == false)
                return false; // if a single case without a mine didn't get revealed the player didnt won yet
        }
    }
    return true; // or else he wins
}

void RevealAllMines(Grid* grid)
{
    for (int i = 0; i < grid->Line; i++)
    {
        for (int j = 0; j < grid->Column; j++)
        {
            if (grid->cells[i][j].GotMine)
                grid->cells[i][j].Reveal = true; // reveal all the mine
        }
    }
}

void ToggleFlag(Grid* grid, int x, int y)
{
    if (x < 0 or x >= grid->Line or y < 0 or y >= grid->Column)
        return;

    Cell* cell = &grid->cells[x][y];

    if (cell->Reveal)
        return;

    cell->MarkedByFlag = !cell->MarkedByFlag; // the player can flag or unflag as he wish
}


void PlayerAction(Grid* grid, int x, int y, int difficulty, bool* gameOver)
{
    if (x < 0 || x >= grid->Line || y < 0 || y >= grid->Column)
        return;

    Cell* cell = &grid->cells[x][y];

    // if the cell is already revealed, we do nothing
    if (cell->Reveal)
    {
        cout << "This case is already revealed." << endl;
        return;
    }

    // we can't reveal a case already flagged
    if (cell->MarkedByFlag)
    {
        cout << "This case is already flagged. Unflag it to reveal." << endl;
        return;
    }

    // if it contains a mine, end of the game
    if (cell->GotMine)
    {
        RevealAllMines(grid);
        DisplayGrid(*grid, difficulty);
        cout << " You hit a mine ! Game over." << endl;
        *gameOver = true;
        return;
    }

    // or else, reveal the case
    RevealCase(grid, x, y);
}


