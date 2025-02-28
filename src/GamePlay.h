#pragma once
#include "Headers.h"
#include "Constants.h"
#include "GameStates.h"
// Directions: Top, Right, Bottom, Left
enum Direction { TOP = 0, RIGHT, BOTTOM, LEFT };

struct Cell {
    int x, y;
    bool visited = false;
    bool walls[4] = { true, true, true, true }; // Top, Right, Bottom, Left
    bool isCorridor = false;  // Flag to indicate this cell should be a corridor
    int hp;
    void Draw() const {
        int x0 = x * CELL_SIZE;
        int y0 = y * CELL_SIZE;
        int x1 = x0 + CELL_SIZE;
        int y1 = y0 + CELL_SIZE;

        // Choose colors for walls and corridors
        Color corridorColor = PURPLE;  // Color for corridors
        Color wallColor = WHITE;          // Color for walls

        // If the cell is a corridor, don't draw walls
        if (isCorridor) {
            for (int i = 0; i < CELL_SIZE / 4; i++)
            {
                // Draw diagonal "stairs" effect (simulating steps)
                DrawLine(x0 + i * 4, y0 + i * 4, x1, y0 + (i + 1) * 4, corridorColor);
                DrawLine(x0 + i * 4, y1 - (i + 1) * 4, x1, y1 - i * 4, corridorColor);
            }
        } else {
            Color wallColor = WHITE;  // Walls have this color
            if (walls[TOP]) DrawLine(x0, y0, x1, y0, wallColor);
            if (walls[RIGHT]) DrawLine(x1, y0, x1, y1, wallColor);
            if (walls[BOTTOM]) DrawLine(x1, y1, x0, y1, wallColor);
            if (walls[LEFT]) DrawLine(x0, y1, x0, y0, wallColor);
            if(hp != 0){

                DrawTexture(hp < 0? fireTexture : grassTexture, x0, y0, WHITE);
                //DrawRectangle(x0, y0, CELL_SIZE, CELL_SIZE, hp < 0? RED:GREEN);
            }
        }
    }
};

std::vector<Cell> grid(COLS * ROWS);
std::stack<Cell*> stack;
Cell* startCell = nullptr;
Cell* endCell = nullptr;
Cell* player = nullptr;
Cell* current = nullptr;
std::queue<Cell*> path;
std::vector<Cell*> validCells; // To store the valid cells player can move to
Color transparentGray = {128, 128, 128, 210};
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
    current = &grid[rand()%(ROWS*COLS)];
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

        if (rand() % 100 < 3) {  // Chance of becoming a corridor cell
            current->isCorridor = true;
            current->hp = 0;
            std::fill(std::begin(current->walls), std::end(current->walls), false); // Remove all walls
        }
        else
        {
            int randInt = rand() % (4+currentLevel);
            if(randInt == 0){
                current->hp = 0;
            }else if(randInt == 1){
                current->hp = 1;
            }else{
                current->hp = -1;
            }
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
// Calculate the Manhattan distance between two cells
int GetDistance(Cell* from, Cell* to) {
    return abs(from->x - to->x) + abs(from->y - to->y);
}

// Highlight and find valid cells the player can move to within a maximum distance
void HighlightValidCells() {
    validCells.clear();
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            Cell* target = GetCell(x, y);
            if (GetDistance(player, target) <= MAX_MOVE_DISTANCE) {
                // Check if there's no wall between the player and the target
                bool canMove = false;
                if (target->x > player->x) canMove = true;
                if (target->x < player->x) canMove = true;
                if (target->y > player->y) canMove = true;
                if (target->y < player->y) canMove = true;

                if (canMove) {
                    validCells.push_back(target);
                }
            }
        }
    }
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
// Handle mouse click to move player within the valid range
void HandleMouseClick() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX() / CELL_SIZE;
        int mouseY = GetMouseY() / CELL_SIZE;
        Cell* target = GetCell(mouseX, mouseY);

        // Check if the clicked target is within the valid movement range
        if (std::find(validCells.begin(), validCells.end(), target) != validCells.end()) {
            path = FindShortestPath(player, target);
        }
    }
}

void MovePlayer() {
    if (!path.empty()) {
        currentScore += path.front()->hp;
        path.front()->hp = 0;

        // Check game over conditions
        if(path.front() == endCell){
            gameOverState = LEVEL_PASSED;
            currentGameState = GameOver;
        }
        
        if(currentScore <= 0){
            currentScore = 0;
            gameOverState = LEVEL_FAILED;
            currentGameState = GameOver;
        }

        path.pop();
        if (!path.empty()) {
            player = path.front();
        }
        
    }
}

void GamePlayHandler()
{
    HighlightValidCells();
        HandleMouseClick();
        MovePlayer();

        BeginDrawing();
        ClearBackground(BLACK);
        
        for (const Cell& cell : grid) {
            cell.Draw();
        }

        // Highlight valid move cells
        if(path.empty()){
            for (Cell* validCell : validCells) 
            {
                DrawRectangle(validCell->x * CELL_SIZE, validCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, transparentGray);
            }
        }

        DrawRectangle(startCell->x * CELL_SIZE, startCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
        DrawRectangle(endCell->x * CELL_SIZE, endCell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);

        DrawTexture(playerTexture, player->x*CELL_SIZE, player->y*CELL_SIZE, WHITE);

        char result[100];
        strcpy(result, "HP: ");
        strcat(result, std::to_string(currentScore).c_str());
        DrawText(result, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT-50, 30, 
        currentScore < 100? RED:GREEN);

        strcpy(result, "Level ");
        strcat(result, std::to_string(currentLevel).c_str());
        DrawText(result, SCREEN_WIDTH/2 + 250, SCREEN_HEIGHT-50, 15, DARKGREEN);

        if(IsKeyDown(KEY_SPACE))
        {
            currentGameState = GameOver;
            InitMaze();
            currentGameState = Gameplay;
        }

        EndDrawing();
}