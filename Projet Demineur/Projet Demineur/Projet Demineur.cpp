#include <iostream>
#include <cstdlib>
#include "Grid.h"
#include "Cell.h"
using namespace std;

int main()
{
    bool playAgain = true;

    while (playAgain) // the player can play as he wants if he answers 'Y' or 'y' at the end 
    {
        int col = 0;
        int line = 0;


        int difficulty = AskDiff(); // intitialization of the game
        Grid grid = InitGrid(difficulty);
        ChooseCase(&grid, &col, &line);
        PlaceMine(&grid, col - 1, line - 1);
        CountAdjacentMine(grid);
        RevealCase(&grid, line - 1, col - 1);

        bool gameOver = false;

        while (!gameOver) // while the party hasn't end yet we repeat the loop
        {
            DisplayGrid(grid, difficulty);

            char action;
            cout << "Choose action : R to reveal, F to flag or unflag" << endl;
            cin >> action;
            if (action != 'r' and action != 'R' and action != 'f' and action != 'F') // we verify the player input
            {
                cout << "Unknown input, try again.";
            }

            cout << "Enter column and line:" << endl;
            cin >> col >> line;

            if (col < 1 or col > grid.Column or line < 1 or line > grid.Line) // we verify the coordinates
            {
                cout << "Invalid coordinates. Try again." << endl;
                continue;
            }

            int x = line - 1;
            int y = col - 1; // convert the player input into an index 

            if (action == 'F' or action == 'f')
            {
                ToggleFlag(&grid, x, y);
            }
            else if (action == 'R' or action == 'r')
            {
                PlayerAction(&grid, x, y, difficulty, &gameOver);

                if (!gameOver and CheckVictory(&grid)) // if the player didn't lose and revealed all the case he wins !
                {
                    DisplayGrid(grid, difficulty);
                    cout << "Congratulations! You won!" << endl;
                    gameOver = true;
                }
            }
        }

        cout << "Do you want to play again? (Y/N): ";
        char again;
        cin >> again;
        if (again != 'y' and again != 'Y' and again != 'n' and again != 'N') // and the player chooses to play again !
        {
            cout << "Unknown command, try again.";
        }


        playAgain = (again == 'Y' or again == 'y');
    }

    return 0;
}
