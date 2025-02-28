#pragma once
#include "raylib.h"
#include "GamePlay.h"
#include "GameStates.h"

// Button struct for easier button handling
struct Button {
    Rectangle rect;
    const char* text;
};

// Function to check if a button is clicked
bool IsButtonClicked(Button button) {
    return (CheckCollisionPointRec(GetMousePosition(), button.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}
void InitializeMazeAndStartLevel(){
    InitializeGameplayValues();
    InitMaze();
    currentGameState = Gameplay;
}