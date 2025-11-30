#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 9 // 9x9 grid
#define TILE_SIZE 50 // in pixels
#define SCORE_FONT_SIZE 32 // font size for score display
#define GAME_DURATION 300.0f // game duration in seconds

bool isGameOver = false;
float gameTimer = GAME_DURATION; 

char board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE] = {0};

int score=0;
Vector2 grid_origin;
Vector2 selected_tile = { -1, -1 };

const int primes[8] = {3, 5, 7, 11, 13, 17, 19, 23};
int target_prime;

Texture2D background;

char random_tile() {
    return (rand()%9)+'1'; 
}

int tile_char_to_int(char c) {
    return c - '0'; 
}

void set_new_target_prime() {
    target_prime = primes[rand() % 8];
}

void swap_tiles(int x1, int y1, int x2, int y2) {
    char temp = board[y1][x1];
    board[y1][x1] = board[y2][x2];
    board[y2][x2] = temp;
}

void shuffle_board() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = random_tile();
        }
    }
}

bool are_tiles_adjacent(Vector2 a, Vector2 b) {
    return (abs((int)a.x - (int)b.x) + abs((int)a.y - (int)b.y)) == 1;
}
// Find matches of three tiles that sum to the target prime
bool find_matches() {
    bool found = false;
    
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            matched[y][x] = false;
        }
    }
//search for horizontal matches
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE - 2; x++) {
            int sum = tile_char_to_int(board[y][x]) +
                      tile_char_to_int(board[y][x + 1]) +
                      tile_char_to_int(board[y][x + 2]);
            
            if (sum == target_prime) {
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;
                found = true;
            }
        }
    }
//search for vertical matches
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE - 2; y++) {
            int sum = tile_char_to_int(board[y][x]) +
                      tile_char_to_int(board[y + 1][x]) +
                      tile_char_to_int(board[y + 2][x]);
            
            if (sum == target_prime) {
                matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;
                found = true;
            }
        }
    }
    
    return found;
}
// Remove matched tiles and let tiles above fall down (cascade)
void resolve_matches() {
    for (int x = 0; x < BOARD_SIZE; x++) {
        int write_y = BOARD_SIZE - 1; 
        
        for (int y = BOARD_SIZE - 1; y >= 0; y--) {
            if (!matched[y][x]) {
                board[write_y][x] = board[y][x];
                write_y--;
            }
        }

        while (write_y >= 0) {
            board[write_y][x] = random_tile();
            write_y--;
        }
    }
}

void init_board() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = random_tile();
        }
    }

    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
    grid_origin = (Vector2){(GetScreenWidth() - grid_width)/2 , (GetScreenHeight() - grid_height)/2};
    set_new_target_prime();

    while (find_matches()) {
        resolve_matches();
    }
    
    score = 0;
}

int main(void) {
    const int screen_width = 1000;
    const int screen_height = 650;

    InitWindow(screen_width, screen_height, "Prime Match-3");
    SetTargetFPS(60);
    srand(time(NULL));

    background = LoadTexture("assets/background.jpg");
    
    init_board();
    Vector2 mouse = { 0, 0 };
    //                              Top left corner x , Top left corner y, w,h
    Rectangle shuffleButton = {screen_width - 160, screen_height - 60, 140, 40};

    while (!WindowShouldClose()) {

        if (!isGameOver)
        {
            gameTimer -= GetFrameTime();
            if (gameTimer <= 0.0f)
            {
                gameTimer = 0.0f;
                isGameOver = true;
            }

            mouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                
                int x = (mouse.x - grid_origin.x) / TILE_SIZE;
                int y = (mouse.y - grid_origin.y) / TILE_SIZE;

                if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                    Vector2 current_tile = (Vector2){ (float)x, (float)y };

                    if (selected_tile.x < 0) {
                        selected_tile = current_tile;
                    } else {
                        if (are_tiles_adjacent(selected_tile, current_tile)) {
                            swap_tiles(selected_tile.x, selected_tile.y, current_tile.x, current_tile.y);

                            if (find_matches()) {
                                score += 10;
                                resolve_matches();
                                
                                while (find_matches()) {
                                    score += 10;
                                    resolve_matches();
                                }
                                
                                set_new_target_prime();

                            } else {
                                swap_tiles(selected_tile.x, selected_tile.y, current_tile.x, current_tile.y);
                            }
                        }
                        selected_tile = (Vector2){ -1, -1 };
                    }
                } 
                else if (CheckCollisionPointRec(mouse, shuffleButton)) {
                    shuffle_board(); 
                    
                    while (find_matches()) {
                        resolve_matches();
                    }
                    
                    selected_tile = (Vector2){ -1, -1 };
                }
                else {
                    selected_tile = (Vector2){ -1, -1 };
                }
            }
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

       DrawTexturePro(
            background,
            (Rectangle){ 0, 0, (float)background.width, (float)background.height }, // source rectangle (part of the texture to use)
            (Rectangle){ 0, 0, (float)screen_width, (float)screen_height }, // destination rectangle (the background is scaled and streched to fit this size)
            (Vector2){ 0, 0 }, // position offset
            0.0f, // rotation angle from origin
            WHITE // colour tint
        );

        if (!isGameOver)
        {
            DrawRectangle(
                grid_origin.x,
                grid_origin.y,
                BOARD_SIZE* TILE_SIZE,
                BOARD_SIZE*TILE_SIZE,
                Fade(DARKGRAY, 0.60f)
            );

            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    Rectangle rect = {grid_origin.x+(x*TILE_SIZE), grid_origin.y+(y*TILE_SIZE), TILE_SIZE, TILE_SIZE};
                    DrawRectangleLinesEx(rect, 1, DARKGRAY);
                    DrawText( TextFormat("%c", board[y][x]), rect.x + (TILE_SIZE / 3) - 2, rect.y + (TILE_SIZE / 4) - 2, SCORE_FONT_SIZE, WHITE);
                }
            }

            if (selected_tile.x >= 0) {
                DrawRectangleLinesEx(
                    (Rectangle){grid_origin.x + (selected_tile.x * TILE_SIZE), grid_origin.y + (selected_tile.y * TILE_SIZE), TILE_SIZE, TILE_SIZE}, 
                    2, 
                    YELLOW);
            }

            DrawText(
                TextFormat("SCORE: %d", score),
                20, 20,
                SCORE_FONT_SIZE,
                YELLOW
            );
            
            DrawText(
                TextFormat("TARGET: %d", target_prime),
                20,
                screen_height - SCORE_FONT_SIZE - 20,
                SCORE_FONT_SIZE,
                LIME
            );

            DrawText(
                TextFormat("TIME: %02d:%02d", (int)gameTimer / 60, (int)gameTimer % 60),
                screen_width - 200, 20,
                SCORE_FONT_SIZE,
                YELLOW
            );

            DrawRectangleRec(shuffleButton, Fade(BLACK, 0.3f));
            DrawRectangleLinesEx(shuffleButton, 2, BLACK);
            DrawText("SHUFFLE", shuffleButton.x + 20, shuffleButton.y + 10, 20, LIME);
        }
        else
        {
            DrawRectangle(0, 0, screen_width, screen_height, Fade(BLACK, 0.7f));
            
            DrawText("TIME'S UP!", 350, 250, 60, MAROON);
            DrawText(TextFormat("FINAL SCORE: %d", score), 340, 320, 40, YELLOW);
            DrawText("Press [ESC] to quit", 380, 380, 20, WHITE);// the window closes automatically when pressed ESC (even in between the game).
        }

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    
    return 0;
}