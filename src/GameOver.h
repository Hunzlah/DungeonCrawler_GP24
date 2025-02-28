#include "Headers.h"
#include "Constants.h"
#include "GameStates.h"

struct Button {
    Rectangle bounds;
    const char *text;
    bool isHovered;
};

// Function to check if a button is clicked
bool IsButtonClicked(Button &button) {
    Vector2 mousePoint = GetMousePosition();
    button.isHovered = CheckCollisionPointRec(mousePoint, button.bounds);

    if (button.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }
    return false;
}
// Buttons
Button nextLevelBtn = { { SCREEN_WIDTH / 2 - 75, 250, 150, 50 }, "Next Level", false };
Button retryBtn = { { SCREEN_WIDTH / 2 - 75, 250, 150, 50 }, "Retry", false };
Button mainMenuBtn = { { SCREEN_WIDTH / 2 - 75, 320, 150, 50 }, "Main Menu", false };


void GameOverHandler(){
    
    BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameOverState == LEVEL_PASSED) {
            DrawText("Level Passed!", SCREEN_WIDTH / 2 - 100, 100, 30, GREEN);
            
            // Draw Next Level Button
            DrawRectangleRec(nextLevelBtn.bounds, nextLevelBtn.isHovered ? DARKGRAY : GRAY);
            DrawText(nextLevelBtn.text, nextLevelBtn.bounds.x + 20, nextLevelBtn.bounds.y + 15, 20, WHITE);
            
            if (IsButtonClicked(nextLevelBtn)) {
                currentLevel += 1;
                currentGameState = Gameplay;
            }
        } 
        else if (gameOverState == LEVEL_FAILED) {
            DrawText("Level Failed!", SCREEN_WIDTH / 2 - 100, 100, 30, RED);
            
            // Draw Retry Button
            DrawRectangleRec(retryBtn.bounds, retryBtn.isHovered ? DARKGRAY : GRAY);
            DrawText(retryBtn.text, retryBtn.bounds.x + 50, retryBtn.bounds.y + 15, 20, WHITE);
            
            if (IsButtonClicked(retryBtn)) {
                currentGameState = Gameplay;
            }
        }

        // Draw Main Menu Button (common for both states)
        DrawRectangleRec(mainMenuBtn.bounds, mainMenuBtn.isHovered ? DARKGRAY : GRAY);
        DrawText(mainMenuBtn.text, mainMenuBtn.bounds.x + 20, mainMenuBtn.bounds.y + 15, 20, WHITE);

        if (IsButtonClicked(mainMenuBtn)) {
            currentGameState = MainMenu;
        }

        EndDrawing();


}