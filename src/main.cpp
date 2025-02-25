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
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator - DFS with Raylib");
    SetTargetFPS(60);

    InitMaze();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

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

        // Draw the maze
        for (const Cell& cell : grid) {
            cell.Draw();
        }

        // Highlight the current cell
        if (current) {
            DrawRectangle(current->x * CELL_SIZE, current->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
