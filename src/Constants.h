#pragma once
#include "raylib.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = 20;
const int COLS = SCREEN_WIDTH / CELL_SIZE;
const int ROWS = SCREEN_HEIGHT / (CELL_SIZE*1.25f);
const int MAX_MOVE_DISTANCE = 5; // Maximum number of tiles the player can move

Texture2D fireTexture;
Texture2D grassTexture;
Texture2D playerTexture;