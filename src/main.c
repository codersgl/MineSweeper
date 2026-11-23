#include "board.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 40
#define GRID_X 100
#define GRID_Y 100

typedef enum { GAME_PLAYING, GAME_WON, GAME_LOST } GameState;

static Rectangle cell_rect(int row, int col) {
  return (Rectangle){
      .x = GRID_X + col * CELL_SIZE,
      .y = GRID_Y + row * CELL_SIZE,
      .width = CELL_SIZE,
      .height = CELL_SIZE,
  };
}

static void draw_bomb(Rectangle r) {
  Vector2 center = {r.x + r.width * 0.5f, r.y + r.height * 0.55f};

  DrawCircleV(center, 10.0f, BLACK);
  DrawCircleV((Vector2){center.x - 4, center.y - 4}, 3.0f, GRAY);
  DrawLineEx((Vector2){center.x + 7, center.y - 8},
             (Vector2){center.x + 12, center.y - 15}, 2.0f, DARKBROWN);
  DrawCircleV((Vector2){center.x + 13, center.y - 16}, 2.5f, ORANGE);
}

int main(void) {
  srand((unsigned int)time(NULL));

  const int screen_w = GRID_X * 2 + COLS * CELL_SIZE;
  const int screen_h = GRID_Y * 2 + ROWS * CELL_SIZE;

  // Initialize the board
  Cell board[ROWS][COLS] = {0};
  bool board_ready = false;
  GameState state = GAME_PLAYING;

  InitWindow(screen_w, screen_h, "MineSweeper");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // ========================= Input =================================
    Vector2 mouse = GetMousePosition();

    int hover_row = -1;
    int hover_col = -1;
    // Calculate which cell is hovered by mouse.
    if (mouse.x >= GRID_X && mouse.x < GRID_X + COLS * CELL_SIZE &&
        mouse.y >= GRID_Y && mouse.y < GRID_Y + ROWS * CELL_SIZE) {
      hover_col = (int)((mouse.x - GRID_X) / CELL_SIZE);
      hover_row = (int)((mouse.y - GRID_Y) / CELL_SIZE);
    }

    // ========================== Update State ==========================

    Rectangle restart_button = {screen_w - 180, screen_h - 60, 160, 40};
    if (state != GAME_PLAYING &&
        CheckCollisionPointRec(mouse, restart_button) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      board_clear(board);
      board_ready = false;
      state = GAME_PLAYING;
    }

    if (state == GAME_PLAYING && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        hover_row >= 0) {
      if (!board_ready) {
        board_init(board, hover_row, hover_col);
        board_ready = true;
      }
      if (reveal_cell(board, hover_row, hover_col)) {
        state = GAME_LOST;
        reveal_all_mines(board);

      } else if (board_is_won(board)) {
        state = GAME_WON;
      }
    }

    if (board_ready && state == GAME_PLAYING &&
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && hover_row >= 0) {
      set_flag(board, hover_row, hover_col);
    }

    // ========================== Render ================================
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw the grid cell, if cell is hoverd by mouse, turn it to light gray.
    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        Rectangle r = cell_rect(row, col);
        Cell *cell = &board[row][col];
        Color fill = cell->revealed ? RAYWHITE : LIGHTGRAY;

        if (cell->flagged) {
          fill = ORANGE;
        }

        if (state == GAME_PLAYING && row == hover_row && col == hover_col) {
          fill = Fade(fill, 0.85f);
        }

        DrawRectangleRec(r, fill);
        DrawRectangleLinesEx(r, 2.0f, DARKGRAY);

        if (cell->revealed) {
          if (cell->mine) {
            draw_bomb(r);
          } else if (cell->adjacent > 0) {
            char text[2] = {(char)('0' + cell->adjacent), '\0'};
            DrawText(text, r.x + 14, r.y + 10, 24, DARKGRAY);
          }
        }
      }
    }

    if (state == GAME_LOST) {
      DrawText("Game Over", 20, screen_h - 50, 30, MAROON);
    } else if (state == GAME_WON) {
      DrawText("You Win!", 20, screen_h - 50, 30, DARKGREEN);
    }

    if (state != GAME_PLAYING) {
      DrawRectangleRec(restart_button, LIGHTGRAY);
      DrawRectangleLinesEx(restart_button, 2.0f, DARKGRAY);
      DrawText("Restart", (int)restart_button.x + 35,
               (int)restart_button.y + 10, 20, DARKGRAY);
    }

    DrawText("MineSweeper", 20, 20, 30, DARKGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
