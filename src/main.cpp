#include "raylib.h"
#include <vector>
#include <stack>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = 20;
const int COLS = SCREEN_WIDTH / CELL_SIZE;
const int ROWS = SCREEN_HEIGHT / CELL_SIZE;

// Directions: Top, Right, Bottom, Left
enum Direction { TOP = 0, RIGHT, BOTTOM, LEFT };

struct Cell {
    int x, y;
    bool visited = false;
    bool walls[4] = { true, true, true, true }; // Top, Right, Bottom, Left

    void Draw() const {
        int x0 = x * CELL_SIZE;
        int y0 = y * CELL_SIZE;
        int x1 = x0 + CELL_SIZE;
        int y1 = y0 + CELL_SIZE;

        Color wallColor = WHITE;
        if (walls[TOP]) DrawLine(x0, y0, x1, y0, wallColor);
        if (walls[RIGHT]) DrawLine(x1, y0, x1, y1, wallColor);
        if (walls[BOTTOM]) DrawLine(x1, y1, x0, y1, wallColor);
        if (walls[LEFT]) DrawLine(x0, y1, x0, y0, wallColor);
    }
};

// Maze grid and DFS stack
std::vector<Cell> grid(COLS * ROWS);
std::stack<Cell*> stack;
Cell* current = nullptr;
Cell* startCell = nullptr;
Cell* endCell = nullptr;

// Player position
int playerX = 0, playerY = 0;
bool gameWon = false;

// Get a cell reference within the grid boundaries
Cell* GetNeighbor(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        return &grid[y * COLS + x];
    return nullptr;
}

// Check for unvisited neighbors
Cell* GetUnvisitedNeighbor(Cell* cell) {
    std::vector<Cell*> neighbors;
    Cell* top = GetNeighbor(cell->x, cell->y - 1);
    Cell* right = GetNeighbor(cell->x + 1, cell->y);
    Cell* bottom = GetNeighbor(cell->x, cell->y + 1);
    Cell* left = GetNeighbor(cell->x - 1, cell->y);

    if (top && !top->visited) neighbors.push_back(top);
    if (right && !right->visited) neighbors.push_back(right);
    if (bottom && !bottom->visited) neighbors.push_back(bottom);
    if (left && !left->visited) neighbors.push_back(left);

    if (!neighbors.empty())
        return neighbors[rand() % neighbors.size()]; // Pick random neighbor
    return nullptr;
}

// Remove walls between two cells
void RemoveWalls(Cell* current, Cell* next) {
    int dx = next->x - current->x;
    int dy = next->y - current->y;

    if (dx == 1) { current->walls[RIGHT] = false; next->walls[LEFT] = false; }   // Move Right
    if (dx == -1) { current->walls[LEFT] = false; next->walls[RIGHT] = false; }   // Move Left
    if (dy == 1) { current->walls[BOTTOM] = false; next->walls[TOP] = false; }   // Move Down
    if (dy == -1) { current->walls[TOP] = false; next->walls[BOTTOM] = false; }   // Move Up
}

// Initialize maze grid and DFS
void InitMaze() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y * COLS + x] = { x, y };
        }
    }
    current = &grid[0]; // Start at top-left
    current->visited = true;
    stack.push(current);

    startCell = &grid[0];                        // Set start point
    endCell = &grid[COLS * ROWS - 1];            // Set end point
    playerX = startCell->x;
    playerY = startCell->y;
}

// Move player if there's no wall
void MovePlayer(int dx, int dy) {
    if (gameWon) return;

    Cell* currentCell = &grid[playerY * COLS + playerX];
    if (dx == 1 && !currentCell->walls[RIGHT]) playerX++;
    if (dx == -1 && !currentCell->walls[LEFT]) playerX--;
    if (dy == 1 && !currentCell->walls[BOTTOM]) playerY++;
    if (dy == -1 && !currentCell->walls[TOP]) playerY--;

    // Check if player reached the goal
    if (playerX == endCell->x && playerY == endCell->y) {
        gameWon = true;
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game - DFS with Player");
    SetTargetFPS(60);

    InitMaze();

    while (!WindowShouldClose()) {
        // Maze generation using DFS
        if (!stack.empty()) {
            Cell* next = GetUnvisitedNeighbor(current);
            if (next) {
                next->visited = true;
                stack.push(current);
                RemoveWalls(current, next);
                current = next;
            } else {
                current = stack.top();
                stack.pop();
            }
        }

        // Handle Player Movement
        if (IsKeyDown(KEY_W)) MovePlayer(0, -1);
        if (IsKeyDown(KEY_S)) MovePlayer(0, 1);
        if (IsKeyDown(KEY_A)) MovePlayer(-1, 0);
        if (IsKeyDown(KEY_D)) MovePlayer(1, 0);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the maze
        for (const Cell& cell : grid) {
            cell.Draw();
        }

        // Highlight the start and end points
        DrawRectangle(startCell->x * CELL_SIZE, startCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
        DrawRectangle(endCell->x * CELL_SIZE, endCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);

        // Draw the player
        DrawRectangle(playerX * CELL_SIZE + 4, playerY * CELL_SIZE + 4, CELL_SIZE - 8, CELL_SIZE - 8, YELLOW);

        // Display win message
        if (gameWon) {
            DrawText("YOU WIN!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 40, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
