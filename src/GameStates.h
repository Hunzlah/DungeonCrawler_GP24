#pragma once
enum GameState
{
    MainMenu,
    Gameplay,
    GameOver
};
enum MenuScreen { MENU, PLAY, HOW_TO_PLAY, CREDITS, EXIT };
enum GameOverState {
    LEVEL_PASSED,
    LEVEL_FAILED
};
GameState currentGameState = MainMenu;
MenuScreen currentMenuScreen = MENU;
GameOverState gameOverState = LEVEL_PASSED;

int defaultScore = 500;
int currentScore;
int currentLevel;

void InitializeGameplayValues(){
    currentScore = defaultScore;
}