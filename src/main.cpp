#include "raylib.h"
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <algorithm>

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

std::vector<Cell> grid(COLS * ROWS);
std::stack<Cell*> stack;
Cell* startCell = nullptr;
Cell* endCell = nullptr;
Cell* player = nullptr;
Cell* current = nullptr;
std::queue<Cell*> path;

Cell* GetCell(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        return &grid[y * COLS + x];
    return nullptr;
}

Cell* GetUnvisitedNeighbor(Cell* cell) {
    std::vector<Cell*> neighbors;
    Cell* top = GetCell(cell->x, cell->y - 1);
    Cell* right = GetCell(cell->x + 1, cell->y);
    Cell* bottom = GetCell(cell->x, cell->y + 1);
    Cell* left = GetCell(cell->x - 1, cell->y);

    if (top && !top->visited) neighbors.push_back(top);
    if (right && !right->visited) neighbors.push_back(right);
    if (bottom && !bottom->visited) neighbors.push_back(bottom);
    if (left && !left->visited) neighbors.push_back(left);

    if (!neighbors.empty())
        return neighbors[rand() % neighbors.size()];
    return nullptr;
}

void RemoveWalls(Cell* current, Cell* next) {
    int dx = next->x - current->x;
    int dy = next->y - current->y;
    if (dx == 1) { current->walls[RIGHT] = false; next->walls[LEFT] = false; }
    if (dx == -1) { current->walls[LEFT] = false; next->walls[RIGHT] = false; }
    if (dy == 1) { current->walls[BOTTOM] = false; next->walls[TOP] = false; }
    if (dy == -1) { current->walls[TOP] = false; next->walls[BOTTOM] = false; }
}

void GenerateMaze() {
    current = &grid[0];
    current->visited = true;
    stack.push(current);
    
    while (!stack.empty()) {
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
}

void InitMaze() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y * COLS + x] = { x, y };
        }
    }
    GenerateMaze();
    startCell = &grid[0];
    endCell = &grid[COLS * ROWS - 1];
    player = startCell;
}

std::queue<Cell*> FindShortestPath(Cell* start, Cell* target) {
    std::unordered_map<Cell*, Cell*> cameFrom;
    std::queue<Cell*> frontier;
    frontier.push(target);
    cameFrom[target] = nullptr;
    
    while (!frontier.empty()) {
        Cell* current = frontier.front();
        frontier.pop();
        if (current == start) break;
        
        std::vector<std::pair<Cell*, Direction>> neighbors = {
            { GetCell(current->x, current->y - 1), TOP },
            { GetCell(current->x + 1, current->y), RIGHT },
            { GetCell(current->x, current->y + 1), BOTTOM },
            { GetCell(current->x - 1, current->y), LEFT }
        };
        
        for (auto& [neighbor, dir] : neighbors) {
            if (neighbor && cameFrom.find(neighbor) == cameFrom.end() && !current->walls[dir]) {
                frontier.push(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }
    
    std::queue<Cell*> path;
    for (Cell* step = start; step; step = cameFrom[step]) path.push(step);
    return path;
}

void HandleMouseClick() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX() / CELL_SIZE;
        int mouseY = GetMouseY() / CELL_SIZE;
        Cell* target = GetCell(mouseX, mouseY);
        if (target) path = FindShortestPath(player, target);
    }
}

void MovePlayer() {
    if (!path.empty()) {
        path.pop();
        if (!path.empty()) {
            player = path.front();
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game - Click to Move");
    SetTargetFPS(60);
    InitMaze();

    while (!WindowShouldClose()) {
        HandleMouseClick();
        MovePlayer();
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (const Cell& cell : grid) {
            cell.Draw();
        }
        
        DrawRectangle(startCell->x * CELL_SIZE, startCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
        DrawRectangle(endCell->x * CELL_SIZE, endCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
        DrawRectangle(player->x * CELL_SIZE + 4, player->y * CELL_SIZE + 4, CELL_SIZE - 8, CELL_SIZE - 8, YELLOW);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
