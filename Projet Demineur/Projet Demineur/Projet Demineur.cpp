#include <iostream>
#include <cstdlib>
#include "Grid.h"
#include "Cell.h"
using namespace std;

int main()
{
    bool playAgain = true;

    while (playAgain)
    {
        int col = 0;
        int line = 0;
        int difficulty = AskDiff();
        Grid grid = InitGrid(difficulty);

        ChooseCase(&grid, &col, &line);
        PlaceMine(&grid, col - 1, line - 1);
        CountAdjacentMine(grid);
        RevealCase(&grid, line - 1, col - 1);

        bool gameOver = false;

        while (!gameOver)
        {
            DisplayGrid(grid, difficulty);

            char action;
            cout << "Choose action: R to reveal, F to flag/unflag" << endl;
            cin >> action;

            cout << "Enter column and line:" << endl;
            cin >> col >> line;

            if (col < 1 || col > grid.Column || line < 1 || line > grid.Line)
            {
                cout << "Invalid coordinates. Try again." << endl;
                continue;
            }

            int x = line - 1;
            int y = col - 1;

            if (action == 'F' || action == 'f')
            {
                ToggleFlag(&grid, x, y);
            }
            else if (action == 'R' || action == 'r')
            {
                PlayerAction(&grid, x, y, difficulty, &gameOver);

                if (!gameOver && CheckVictory(&grid))
                {
                    DisplayGrid(grid, difficulty);
                    cout << "🎉 Congratulations! You won!" << endl;
                    gameOver = true;
                }
            }
            else
            {
                cout << "Unknown action. Use R or F." << endl;
            }
        }

        cout << "Do you want to play again? (Y/N): ";
        char again;
        cin >> again;
        playAgain = (again == 'Y' || again == 'y');
    }

    return 0;
}
