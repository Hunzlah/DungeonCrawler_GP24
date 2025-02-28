#include "GamePlay.h"
#include "MainMenu.h"
#include "GameStates.h"
#include "GameOver.h"

int main()
{
    InitializeGameplayValues();
    currentLevel = 1;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game - Click to Move");
    SetTargetFPS(60);
    InitMaze();
    bool isManualExit = false;
    while (!WindowShouldClose())
    {
        switch (currentGameState)
        {
        case MainMenu:
        if (currentMenuScreen == EXIT)
            {
                isManualExit = true;
            }
            else
            {
                MainMenuHandler();
            }
            break;
        case Gameplay:
            GamePlayHandler();
            break;
        case GameOver:
            GameOverHandler();
            break;

        default:
            break;
        }
        if(isManualExit) break;
    }

    CloseWindow();
    return 0;
}
