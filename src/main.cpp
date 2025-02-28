#include "GamePlay.h"
#include "MainMenu.h"
#include "GameStates.h"
#include "GameOver.h"
#include "resource_dir.h"
int main()
{
    
    InitializeGameplayValues();
    currentLevel = 1;

    

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game - Click to Move");
    SearchAndSetResourceDir("resources");
    fireTexture = LoadTexture("fire.png");
    grassTexture = LoadTexture("grass.png");
    playerTexture = LoadTexture("player.png");
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

    UnloadTexture(grassTexture);
    UnloadTexture(fireTexture);
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}
