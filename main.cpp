#include <raylib.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#define BOARD_SIZE 3

typedef enum { EMPTY, PLAYER_X, PLAYER_O } Cell;
Cell board[BOARD_SIZE][BOARD_SIZE];
Cell currentPlayer;
int playerXScore = 0;
int playerOScore = 0;
int gameState = 0;
int gameMode = 0;
char winnerMessage[50];
int totalGames = 0;
Texture2D xTexture;
Texture2D oTexture;
Texture2D menuBackground;

void DrawBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Rectangle cell = { 200 + j * 100, 100 + i * 100, 100, 100 };
            DrawRectangleRec(cell, LIGHTGRAY);

            float scaleX = 100.0f / xTexture.width;
            float scaleY = 100.0f / xTexture.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            if (board[i][j] == PLAYER_X) {

                Vector2 position = { cell.x + (cell.width - (xTexture.width * scale)) / 2,
                                     cell.y + (cell.height - (xTexture.height * scale)) / 2 };
                DrawTextureEx(xTexture, position, 0.0f, scale, WHITE);
            }
            else if (board[i][j] == PLAYER_O) {

                scaleX = 100.0f / oTexture.width;
                scaleY = 100.0f / oTexture.height;
                scale = (scaleX < scaleY) ? scaleX : scaleY;

                Vector2 position = { cell.x + (cell.width - (oTexture.width * scale)) / 2,
                                     cell.y + (cell.height - (oTexture.height * scale)) / 2 };
                DrawTextureEx(oTexture, position, 0.0f, scale, WHITE);
            }
        }
    }
}


void ResetBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    currentPlayer = PLAYER_X;
}

int CheckWinner() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) return board[0][2];

    int filledCells = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != EMPTY) filledCells++;
        }
    }
    return (filledCells == BOARD_SIZE * BOARD_SIZE) ? -1 : EMPTY;
}

void ComputerMove() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O;
                if (CheckWinner() == PLAYER_O) return;
                board[i][j] = EMPTY;
            }
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                if (CheckWinner() == PLAYER_X) {
                    board[i][j] = PLAYER_O;
                    return;
                }
                board[i][j] = EMPTY;
            }
        }
    }


    int x, y;
    do {
        x = GetRandomValue(0, BOARD_SIZE - 1);
        y = GetRandomValue(0, BOARD_SIZE - 1);
    } while (board[y][x] != EMPTY);

    board[y][x] = PLAYER_O;
}

void DrawMenu() {
    DrawTexture(menuBackground, 0, 0, WHITE);
    DrawText("Tic-Tac-Toe", 300, 50, 40, BLACK);
    DrawText("Press [1] for Play with AI", 200, 150, 20, DARKGRAY);
    DrawText("Press [2] for Play with Player", 200, 200, 20, DARKGRAY);
    DrawText("Press [ESC] to Exit", 200, 250, 20, DARKGRAY);
    DrawText("Score", 300, 300, 30, BLACK);
    DrawText(TextFormat("Player X: %d", playerXScore), 300, 350, 20, RED);
    DrawText(TextFormat("Player O: %d", playerOScore), 300, 390, 20, BLUE);
    DrawText(TextFormat("Total Games: %d", totalGames), 300, 430, 20, DARKGRAY);
}

void DrawGameOver() {
    DrawText(winnerMessage, 300, 50, 40, BLACK);
    DrawText("Press [ENTER] to play again", 200, 150, 20, DARKGRAY);
}

void DrawDraw() {
    DrawText("It's a Draw!", 300, 50, 40, BLACK);
    DrawText("Press [ENTER] to play again", 200, 150, 20, DARKGRAY);
}
int main() {
    InitWindow(800, 600, "Tic-Tac-Toe");
    SetTargetFPS(60);

    xTexture = LoadTexture("C:/Users/lenovo/Documents/images/Снимок экрана 2025-01-25 164842.png");
    oTexture = LoadTexture("C:/Users/lenovo/Documents/images/Снимок экрана 2025-01-25 164859.png");
    menuBackground = LoadTexture("C:/Users/lenovo/Documents/images/1660149279_2-kartinkin-net-p-fon-dlya-menyu-igri-krasivo-2.png");

    ResetBoard();
    gameState = 0;

    while (!WindowShouldClose()) {
    
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        switch (gameState) {
        case 0:
            DrawMenu();
            if (IsKeyPressed(KEY_ONE)) {
                gameMode = 0;
                ResetBoard();
                gameState = 2;
            }
            else if (IsKeyPressed(KEY_TWO)) {
                gameMode = 1;
                ResetBoard();
                gameState = 2;
            }
            break;

        case 2:
            DrawBoard();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int row = (mousePos.y - 100) / 100;
                int col = (mousePos.x - 200) / 100;

                if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == EMPTY) {
                    board[row][col] = currentPlayer;
                    int winner = CheckWinner();
                    if (winner == PLAYER_X) {
                        playerXScore++;
                        sprintf(winnerMessage, "Player X wins!");
                        totalGames++;
                        gameState = 3;
                    }
                    else if (winner == PLAYER_O) {
                        playerOScore++;
                        sprintf(winnerMessage, "Player O wins!");
                        totalGames++;
                        gameState = 3;
                    }
                    else if (winner == -1) {
                        totalGames++;
                        gameState = 4;
                    }
                    else {
                        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;

                        if (gameMode == 0 && currentPlayer == PLAYER_O) {
                            ComputerMove();
                            int winner = CheckWinner();
                            if (winner == PLAYER_O) {
                                playerOScore++;
                                sprintf(winnerMessage, "Player O wins!");
                                gameState = 3;
                            }
                            else if (winner == -1) {
                                gameState = 4;
                            }
                            currentPlayer = PLAYER_X;
                        }
                    }
                }
            }
            break;

        case 3:
            DrawGameOver();
            if (IsKeyPressed(KEY_ENTER)) {
                ResetBoard();
                gameState = 0;
            }
            break;

        case 4:
            DrawDraw();
            if (IsKeyPressed(KEY_ENTER)) {
                ResetBoard();
                gameState = 0;
            }
            break;
        }

        EndDrawing();
    }

    UnloadTexture(xTexture);
    UnloadTexture(oTexture);
    UnloadTexture(menuBackground);

    CloseWindow();
    return 0;
}
