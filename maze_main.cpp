#include <iostream>
#include <raylib.h>
#include "class.h"

using namespace std;

int screen_width = 900;
int screen_height = 800;
int cell_size = 40;
int grid_width = 10; 
int grid_height = 7; 
bool gamewin = false;
int score; 
float startTime;
float elapsedTime; 
int mazeOffsetX = (screen_width - grid_width * cell_size) / 2;
int mazeOffsetY = (screen_height - grid_height * cell_size) / 2; 

float levelScores[3] = {0}; 
int currentLevelIndex = 0;  
float timeSpent = 0;        
float totalScore = 0;   
float obstacleUpdateInterval = 0.5f; 
float obstacleTimer = 0.0f;

// Function prototypes
void InitializeMaze();
void GenerateMaze(int x, int y);
void DrawMaze();
void DrawPlayer();
void DrawGoal();
void movePlayer(int dx, int dy);
void NextLevel();
  GameState gameState=MENU;


Cell maze[20][15]; 
vector<Obstacle> obstacles;
Level currentLevel = FACILE;

Position player;
Position goal;


Texture2D jerryTexture;
Texture2D cheeseTexture;

// Initialize maze
void InitializeMaze() {
    for (int x = 0; x < grid_width; x++) {
        for (int y = 0; y < grid_height; y++) {
            maze[x][y].visited = false;
            maze[x][y].topWall = true;
            maze[x][y].bottomWall = true;
            maze[x][y].leftWall = true;
            maze[x][y].rightWall = true;
        }
    }
}


void InitializeObstacles() {
    obstacles.clear(); 

    // mobile obstacles
    obstacles.push_back(Obstacle(3, 3, 1, 0, RED));  
    obstacles.push_back(Obstacle(5, 5, 0, 1, BLUE)); 
    obstacles.push_back(Obstacle(7, 2, -1, 0, GREEN)); 
}

// Generate the maze using (Prim-like)
void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;
    while (true) {
        int directions[] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++) {
            int j = GetRandomValue(i, 3);
            int temp = directions[i];
            directions[i] = directions[j];
            directions[j] = temp;
        }

        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int nx = x, ny = y;
            if (directions[i] == 0) ny -= 1;
            else if (directions[i] == 1) nx += 1;
            else if (directions[i] == 2) ny += 1;
            else if (directions[i] == 3) nx -= 1;

            if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height && !maze[nx][ny].visited) {
                if (directions[i] == 0) { // Up
                    maze[x][y].topWall = false;
                    maze[nx][ny].bottomWall = false;
                } else if (directions[i] == 1) { // Right
                    maze[x][y].rightWall = false;
                    maze[nx][ny].leftWall = false;
                } else if (directions[i] == 2) { // Down
                    maze[x][y].bottomWall = false;
                    maze[nx][ny].topWall = false;
                } else if (directions[i] == 3) { // Left
                    maze[x][y].leftWall = false;
                    maze[nx][ny].rightWall = false;
                }

                GenerateMaze(nx, ny);
                moved = true;
                break;
            }
        }
        if (!moved) break;
    }
}


// Draw the maze
void DrawMaze() {
    for (int x = 0; x < grid_width; x++) {
        for (int y = 0; y < grid_height; y++) {
            int posX = mazeOffsetX + x * cell_size;
            int posY = mazeOffsetY + y * cell_size;
            if (maze[x][y].topWall)
                DrawLine(posX, posY, posX + cell_size, posY, WHITE);
            if (maze[x][y].rightWall)
                DrawLine(posX + cell_size, posY, posX + cell_size, posY + cell_size, WHITE);
            if (maze[x][y].bottomWall)
                DrawLine(posX, posY + cell_size, posX + cell_size, posY + cell_size, WHITE);
            if (maze[x][y].leftWall)
                DrawLine(posX, posY, posX, posY + cell_size, WHITE);
        }
    }
}


// Draw the player 
void DrawPlayer() {
    float scale = (float)cell_size / jerryTexture.width;
    Vector2 position = {
        mazeOffsetX + (float)(player.x * cell_size), 
        mazeOffsetY + (float)(player.y * cell_size)
    };
    DrawTextureEx(jerryTexture, position, 0.0f, scale, WHITE);
}

void DrawGoal() {
    float scale = (float)cell_size / cheeseTexture.width;
    Vector2 position = {
        mazeOffsetX + (float)(goal.x * cell_size), 
        mazeOffsetY + (float)(goal.y * cell_size)
    };
    DrawTextureEx(cheeseTexture, position, 0.0f, scale, WHITE);
}



// Move the playerr
void movePlayer(int dx, int dy) {
    int moveX = player.x + dx;
    int moveY = player.y + dy;
    if (moveX < 0 || moveX >= grid_width || moveY < 0 || moveY >= grid_height) return;
    if (dx == -1 && maze[player.x][player.y].leftWall) return;
    if (dx == 1 && maze[player.x][player.y].rightWall) return;
    if (dy == -1 && maze[player.x][player.y].topWall) return;
    if (dy == 1 && maze[player.x][player.y].bottomWall) return;
for (const auto& obstacle : obstacles) {
        if (moveX == obstacle.x && moveY == obstacle.y) {
            gameState = GAMEOVER; 
            return;
        }
    }
    player.x = moveX;
    player.y = moveY;

   

    if (player.x == goal.x && player.y == goal.y) gamewin = true;
}


// Update to next level
void NextLevel() {
    if (currentLevel == FACILE) {
        currentLevel = MOYEN;
        grid_width = 15;
        grid_height = 10;
    } else if (currentLevel == MOYEN) {
        currentLevel = DIFFICULTE;
        grid_width = 20;
        grid_height = 15;
    }

    InitializeMaze();
    GenerateMaze(0, 0);
    player.x = 0;
    player.y = 0;
    goal.x = grid_width - 1;
    goal.y = grid_height - 1;    
    startTime = GetTime(); 
     if (currentLevel == DIFFICULTE) {
        InitializeObstacles(); 
    }
   
mazeOffsetX = (screen_width - grid_width * cell_size) / 2;
mazeOffsetY = (screen_height - grid_height * cell_size) / 2;

}
void UpdateObstacles() {
    float currentTime = GetTime();
    if (currentTime - obstacleTimer >= obstacleUpdateInterval) {
        obstacleTimer = currentTime; 
        for (auto& obstacle : obstacles) {
            obstacle.x += obstacle.dx;
            obstacle.y += obstacle.dy;

            
            if (obstacle.x < 0 || obstacle.x >= grid_width) {
                obstacle.dx = -obstacle.dx;
                obstacle.x += obstacle.dx; 
            }
            if (obstacle.y < 0 || obstacle.y >= grid_height) {
                obstacle.dy = -obstacle.dy;
                obstacle.y += obstacle.dy; 
            }
            if (player.x == obstacle.x && player.y == obstacle.y) {
                gameState = GAMEOVER; 
        }
    }
}
}

void DrawObstacles() {
    for (const auto& obstacle : obstacles) {
        int posX = mazeOffsetX + obstacle.x * cell_size;
        int posY = mazeOffsetY + obstacle.y * cell_size;
        DrawRectangle(posX, posY, cell_size, cell_size, obstacle.color);
    }
}




int main() {


    InitWindow(screen_width, screen_height, "Maze Generator");
    SetTargetFPS(60);

    // Game setup
    jerryTexture = LoadTexture("jerry.png");
    cheeseTexture = LoadTexture("cheese.png");

    // erreur for taxtures that represents our player and goal
    if (jerryTexture.id == 0 || cheeseTexture.id == 0) {
        cout << "Error: Could not load textures." << endl;
        CloseWindow();
        return -1;
    }

    while (!WindowShouldClose()) {
        
        //screen menu
    if (gameState == MENU) {
    BeginDrawing();
    ClearBackground(BLACK);

    // the title
    const char* title = "Welcome to the Maze Game!";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, screen_width / 2 - titleWidth / 2, screen_height / 2 - 100, 30, WHITE);

    // Play button
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = screen_width / 2 - buttonWidth / 2;
    int buttonY = screen_height / 2;

   
    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, GRAY);

   
    const char* playText = "Play";
    int playTextWidth = MeasureText(playText, 30);
    int playTextX = buttonX + (buttonWidth - playTextWidth) / 2;
    int playTextY = buttonY + (buttonHeight - 30) / 2; 
    DrawText(playText, playTextX, playTextY, 30, BLACK);

    // Check if the Play button is clicked
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
            mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight) {
            gameState = PLAYING;
            InitializeMaze();
            GenerateMaze(0, 0);
            player.x = 0;
            player.y = 0;
            goal.x = grid_width - 1;
            goal.y = grid_height - 1;
              if (currentLevel == DIFFICULTE) {
            InitializeObstacles();
        }
        }
    }

    EndDrawing();
}


        // Game Playing Screen
    else if (gameState == PLAYING) {
    if (gamewin) {
     gameState = LEVEL_COMPLETE; 
    gamewin = false; 
    timeSpent = GetTime() - startTime;
    levelScores[currentLevelIndex] = 1000.0f / timeSpent;
}
   UpdateObstacles();

    BeginDrawing();
    ClearBackground(BLACK);

    
    if (currentLevel == FACILE) {
        DrawText("Level: Facile", 10, 10, 20, GREEN);
    } else if (currentLevel == MOYEN) {
        DrawText("Level: Moyen", 10, 10, 20, YELLOW);
    } else if (currentLevel == DIFFICULTE) {
        DrawText("Level: Difficulte", 10, 10, 20, RED);
    }

     
    elapsedTime = GetTime() - startTime;
    DrawText(TextFormat("Time: %.2f sec", elapsedTime), 10, 40, 20, WHITE);

    DrawMaze();
    DrawPlayer();
    DrawGoal();

    // level dificulte

  if (currentLevel == DIFFICULTE) {
        DrawObstacles();
    }
    // Restart button
    DrawRectangle(screen_width - 120, 10, 100, 40, DARKGRAY);
    DrawText("Restart", screen_width - 110, 20, 20, WHITE);

    // Check if Restart button is clicked
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= screen_width - 120 && mousePos.x <= screen_width - 20 &&
            mousePos.y >= 10 && mousePos.y <= 50) {
            
            InitializeMaze();
            GenerateMaze(0, 0);
            player.x = 0;
            player.y = 0;
            goal.x = grid_width - 1;
            goal.y = grid_height - 1;
            startTime = GetTime();
        }
    }

    EndDrawing();

    
    if (IsKeyDown(KEY_RIGHT)) movePlayer(1, 0);
    if (IsKeyDown(KEY_LEFT)) movePlayer(-1, 0);
    if (IsKeyDown(KEY_UP)) movePlayer(0, -1);
    if (IsKeyDown(KEY_DOWN)) movePlayer(0, 1);
}


else if (gameState == LEVEL_COMPLETE) {
    BeginDrawing();
    ClearBackground(BLACK);

    
    DrawText("Level Complete!", screen_width / 2 - 150, screen_height / 2 - 100, 30, GREEN);

    
    DrawText(TextFormat("Level Score: %.2f", levelScores[currentLevelIndex]), 
             screen_width / 2 - 150, screen_height / 2 - 50, 20, YELLOW);

    
    int buttonWidth = 200;
    int buttonHeight = 50;

    // replay button
    int replayButtonX = screen_width / 2 - buttonWidth - 10;
    int replayButtonY = screen_height / 2;
    DrawRectangle(replayButtonX, replayButtonY, buttonWidth, buttonHeight, GRAY);
    DrawText("Replay Level", replayButtonX + 20, replayButtonY + 15, 20, BLACK);

    // Next  button
   if (currentLevel == DIFFICULTE) {
        
        int finishButtonX = screen_width / 2 + 10;
        int finishButtonY = screen_height / 2;
        DrawRectangle(finishButtonX, finishButtonY, buttonWidth, buttonHeight, GRAY);
        DrawText("Finish", finishButtonX + 50, finishButtonY + 15, 20, BLACK);

        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            if (mousePos.x >= finishButtonX && mousePos.x <= finishButtonX + buttonWidth &&
                mousePos.y >= finishButtonY && mousePos.y <= finishButtonY + buttonHeight) {
                gameState = WIN; 
                for (int i = 0; i < 3; i++) {
                    totalScore += levelScores[i];
                }
            }
        }
    } else {
        
        int nextButtonX = screen_width / 2 + 10;
        int nextButtonY = screen_height / 2;
        DrawRectangle(nextButtonX, nextButtonY, buttonWidth, buttonHeight, GRAY);
        DrawText("Next Level", nextButtonX + 30, nextButtonY + 15, 20, BLACK);

        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            if (mousePos.x >= nextButtonX && mousePos.x <= nextButtonX + buttonWidth &&
                mousePos.y >= nextButtonY && mousePos.y <= nextButtonY + buttonHeight) {
                currentLevelIndex++;
                NextLevel(); 
                gameState = PLAYING;
            }
        }
    }

    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= replayButtonX && mousePos.x <= replayButtonX + buttonWidth &&
            mousePos.y >= replayButtonY && mousePos.y <= replayButtonY + buttonHeight) {
            InitializeMaze();
            GenerateMaze(0, 0);
            player.x = 0;
            player.y = 0;
            goal.x = grid_width - 1;
            goal.y = grid_height - 1;
            startTime = GetTime();
            gameState = PLAYING; 
        }
    }

    EndDrawing();
}

        // Win Screen
      else if (gameState == WIN) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("YOU WIN !", screen_width / 2 - 100, screen_height / 2 - 80, 30, RED);
    DrawText("Your Scores:", screen_width / 2 - 100, screen_height / 2 - 40, 20, YELLOW);

    for (int i = 0; i < 3; i++) {
        DrawText(TextFormat("Level %d: %.2f", i + 1, levelScores[i]),
                 screen_width / 2 - 100, screen_height / 2 + (i * 20), 20, WHITE);
    }

    DrawText(TextFormat("Total Score: %.2f", totalScore),
             screen_width / 2 - 100, screen_height / 2 + 80, 25, GREEN);

    EndDrawing();
} 
else if (gameState == GAMEOVER) {
    BeginDrawing();
    ClearBackground(BLUE);

    DrawText("Game Over!", screen_width / 2 - 100, screen_height / 2 - 50, 30, RED);
    DrawText("You Lost!", screen_width / 2 - 80, screen_height / 2, 20, WHITE);

    // Draw a "Restart" button
    int buttonWidth = 200, buttonHeight = 50;
    int buttonX = screen_width / 2 - buttonWidth / 2;
    int buttonY = screen_height / 2 + 50;

    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, DARKGRAY);
    DrawText("Restart", buttonX + 60, buttonY + 10, 30, WHITE);

    // Check if the "Restart" button is clicked
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
            mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight) {
            // Restart the game
            currentLevel = FACILE;
            currentLevelIndex = 0;
            InitializeMaze();
            GenerateMaze(0, 0);
            player.x = 0;
            player.y = 0;
            goal.x = grid_width - 1;
            goal.y = grid_height - 1;
            startTime = GetTime();
            gameState = PLAYING;
        }
    }

    EndDrawing();
}

    }
    


   
    // textures
    UnloadTexture(jerryTexture);
    UnloadTexture(cheeseTexture);

    CloseWindow();
    return 0;
}
