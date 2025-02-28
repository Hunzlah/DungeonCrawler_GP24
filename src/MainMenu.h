#include "Headers.h"
#include "Constants.h"
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

// Set initial game screen


// Define buttons
Button playButton = {{SCREEN_WIDTH / 2 - 100, 200, 200, 50}, "Play"};
Button howToPlayButton = {{SCREEN_WIDTH / 2 - 100, 270, 200, 50}, "How to Play"};
Button creditsButton = {{SCREEN_WIDTH / 2 - 100, 340, 200, 50}, "Credits"};
Button quitButton = {{SCREEN_WIDTH / 2 - 100, 410, 200, 50}, "Quit"};
Button backButton = {{SCREEN_WIDTH / 2 - 100, 500, 200, 50}, "Back"};

void MainMenuHandler(){
    BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentMenuScreen) {
            case MENU:
                DrawText("Main Menu", SCREEN_WIDTH / 2 - 90, 100, 30, DARKGRAY);
                
                DrawRectangleRec(playButton.rect, LIGHTGRAY);
                DrawText(playButton.text, playButton.rect.x + 50, playButton.rect.y + 15, 20, DARKGRAY);
                
                DrawRectangleRec(howToPlayButton.rect, LIGHTGRAY);
                DrawText(howToPlayButton.text, howToPlayButton.rect.x + 20, howToPlayButton.rect.y + 15, 20, DARKGRAY);
                
                DrawRectangleRec(creditsButton.rect, LIGHTGRAY);
                DrawText(creditsButton.text, creditsButton.rect.x + 50, creditsButton.rect.y + 15, 20, DARKGRAY);
                
                DrawRectangleRec(quitButton.rect, LIGHTGRAY);
                DrawText(quitButton.text, quitButton.rect.x + 70, quitButton.rect.y + 15, 20, DARKGRAY);
                
                if (IsButtonClicked(playButton)) currentMenuScreen = PLAY;
                if (IsButtonClicked(howToPlayButton)) currentMenuScreen = HOW_TO_PLAY;
                if (IsButtonClicked(creditsButton)) currentMenuScreen = CREDITS;
                if (IsButtonClicked(quitButton)) currentMenuScreen = EXIT;
                break;
                
            case HOW_TO_PLAY:
                DrawText("How to Play", SCREEN_WIDTH / 2 - 100, 100, 30, DARKGRAY);
                DrawText("Use the mouse to navigate and click to select.", 100, 200, 20, DARKGRAY);
                DrawRectangleRec(backButton.rect, LIGHTGRAY);
                DrawText(backButton.text, backButton.rect.x + 70, backButton.rect.y + 15, 20, DARKGRAY);
                if (IsButtonClicked(backButton)) currentMenuScreen = MENU;
                break;
                
            case CREDITS:
                DrawText("Credits", SCREEN_WIDTH / 2 - 60, 100, 30, DARKGRAY);
                DrawText("Created by: Hunzlah Bin Saghir", 100, 200, 20, DARKGRAY);
                DrawRectangleRec(backButton.rect, LIGHTGRAY);
                DrawText(backButton.text, backButton.rect.x + 70, backButton.rect.y + 15, 20, DARKGRAY);
                if (IsButtonClicked(backButton)) currentMenuScreen = MENU;
                break;
                
            case PLAY:
                currentMenuScreen = MENU;
                currentGameState = Gameplay;
                break;
                
            case EXIT:
                
                break;
        }

        EndDrawing();
}