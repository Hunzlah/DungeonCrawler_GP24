#pragma once
enum MenuScreen { MENU, PLAY, HOW_TO_PLAY, CREDITS, EXIT };
enum GameState
{
    MainMenu,
    Gameplay,
    GameOver
};

GameState currentGameState = MainMenu;
MenuScreen currentMenuScreen = MENU;
