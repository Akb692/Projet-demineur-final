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

    while (*col < 0 || *col > grid->Column || *line < 0 || *line > grid->Line)
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
        int x = rand() % grid->Line;
        int y = rand() % grid->Column;

        if (!grid->cells[x][y].GotMine && x != FirstX && y != FirstY)
        {
            grid->cells[x][y].GotMine = true;
        }
    }
}

Grid InitGrid(int diff)
{
    Grid grid;

    switch (diff)
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

    grid.TotalMine = grid.Column * grid.Line * 0.15;

    grid.cells = (Cell**)SafeAlloc(sizeof(Cell*) * grid.Line);
    for (int i = 0; i < grid.Line; ++i)
    {
        grid.cells[i] = (Cell*)SafeAlloc(sizeof(Cell) * grid.Column);
    }

    for (int i = 0; i < grid.Line; i++)

    {
        for (int j = 0; j < grid.Column; j++)
        {
            grid.cells[i][j] = InitCell(grid.cells[i][j]);
        }
    }

    return grid;
}

int CountAdjacentMine(Grid grid)
{
    int LineX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int ColumnY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < grid.Line; i++)
    {
        for (int j = 0; j < grid.Column; j++)
        {
            if (grid.cells[i][j].GotMine == false)
            {
                int count = 0;

                for (int k = 0; k < 8; k++)
                {
                    int ni = i + LineX[k];
                    int nj = j + ColumnY[k];


                    if (ni >= 0 and ni < grid.Line and nj >= 0 and nj < grid.Column)
                    {
                        if (grid.cells[ni][nj].GotMine)
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

    if (x < 0 || x >= grid->Line || y < 0 || y >= grid->Column)
        return;

    Cell cell = grid->cells[x][y];


    if (cell.Reveal or cell.MarkedByFlag)
        return;

    grid->cells[x][y].Reveal = true;

    if (cell.GotMine)
        return;


    if (cell.AdjacentMine == 0)
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
                return false;
        }
    }
    return true;
}

void RevealAllMines(Grid* grid)
{
    for (int i = 0; i < grid->Line; i++)
    {
        for (int j = 0; j < grid->Column; j++)
        {
            if (grid->cells[i][j].GotMine)
                grid->cells[i][j].Reveal = true;
        }
    }
}

void ToggleFlag(Grid* grid, int x, int y)
{
    if (x < 0 || x >= grid->Line || y < 0 || y >= grid->Column)
        return;

    Cell* cell = &grid->cells[x][y];

    if (cell->Reveal)
        return;

    cell->MarkedByFlag = !cell->MarkedByFlag;
}


void PlayerAction(Grid* grid, int x, int y, int difficulty, bool* gameOver)
{
    // Vérifie que les coordonnées sont valides
    if (x < 0 || x >= grid->Line || y < 0 || y >= grid->Column)
        return;

    Cell* cell = &grid->cells[x][y];

    // Si la case est déjà révélée, ne rien faire
    if (cell->Reveal)
    {
        cout << "This case is already revealed." << endl;
        return;
    }

    // Si la case est marquée par un drapeau, empêcher la révélation
    if (cell->MarkedByFlag)
    {
        cout << "This case is flagged. Unflag it first to reveal." << endl;
        return;
    }

    // Si la case contient une mine, fin de partie
    if (cell->GotMine)
    {
        RevealAllMines(grid);
        DisplayGrid(*grid, difficulty);
        cout << "?? You hit a mine! Game over." << endl;
        *gameOver = true;
        return;
    }

    // Sinon, révéler la case
    RevealCase(grid, x, y);
}


