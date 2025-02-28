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

int defaultScore = 170;
int currentScore;
int currentLevel;

void InitializeGameplayValues(){
    currentScore = defaultScore - (currentLevel*20) < 50 ? 50 : defaultScore - (currentLevel*5);
}