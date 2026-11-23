#include "board.h"
#include <stdbool.h>
#include <stdlib.h>

void board_clear(Cell board[ROWS][COLS]) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      board[row][col] = (Cell){0};
    }
  }
}

static bool in_board(int row, int col) {
  return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

static bool in_safe_zone(int row, int col, int safe_row, int safe_col) {
  return in_board(safe_row, safe_col)
             ? abs(row - safe_row) <= 1 && abs(col - safe_col) <= 1
             : false;
}

void board_init(Cell board[ROWS][COLS], int safe_row, int safe_col) {
  board_clear(board);

  int placed = 0;
  while (placed < NUMBER_OF_MINES) {
    int row = rand() % ROWS;
    int col = rand() % COLS;

    if (board[row][col].mine) {
      continue;
    }

    if (in_safe_zone(row, col, safe_row, safe_col)) {
      continue;
    }

    board[row][col].mine = true;
    placed++;
  }

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (board[row][col].mine) {
        continue;
      }

      int count = 0;
      for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
          // Pass itself
          if (dr == 0 && dc == 0) {
            continue;
          }

          int nr = row + dr;
          int nc = col + dc;

          if (in_board(nr, nc) && board[nr][nc].mine) {
            count++;
          }
        }
      }

      board[row][col].adjacent = (unsigned char)(count);
    }
  };
}

bool reveal_cell(Cell board[ROWS][COLS], int hover_row, int hover_col) {
  if (!in_board(hover_row, hover_col)) {
    return false;
  }

  Cell *cell = &board[hover_row][hover_col];
  if (cell->revealed || cell->flagged) {
    return false;
  }

  if (cell->mine) {
    cell->revealed = true;
    return true;
  }

  cell->revealed = true;

  Position stack[ROWS * COLS];
  int top = 0;
  if (cell->adjacent == 0) {
    stack[top++] = (Position){hover_row, hover_col};
  }

  while (top > 0) {
    Position current = stack[--top];

    for (int dr = -1; dr <= 1; dr++) {
      for (int dc = -1; dc <= 1; dc++) {
        if (dr == 0 && dc == 0) {
          continue;
        }

        int nr = current.row + dr;
        int nc = current.col + dc;

        if (!in_board(nr, nc)) {
          continue;
        }

        Cell *neighbor = &board[nr][nc];
        if (neighbor->revealed || neighbor->flagged || neighbor->mine) {
          continue;
        }

        neighbor->revealed = true;
        if (neighbor->adjacent == 0) {
          stack[top++] = (Position){nr, nc};
        }
      }
    }
  }

  return false;
}

void set_flag(Cell board[ROWS][COLS], int hover_row, int hover_col) {
  if (!in_board(hover_row, hover_col)) {
    return;
  }

  Cell *cell = &board[hover_row][hover_col];

  if (cell->revealed) {
    return;
  }

  cell->flagged = !cell->flagged;
}

void reveal_all_mines(Cell board[ROWS][COLS]) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (board[row][col].mine) {
        board[row][col].revealed = true;
        board[row][col].flagged = false;
      }
    }
  };
}

bool board_is_won(Cell board[ROWS][COLS]) {
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (!board[row][col].mine && !board[row][col].revealed) {
        return false;
      }
    }
  }
  return true;
}
