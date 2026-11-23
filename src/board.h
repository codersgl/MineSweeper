#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef struct {
  bool mine;
  bool revealed;
  bool flagged;
  unsigned char adjacent;
} Cell;

typedef struct {
  int row;
  int col;
} Position;

#define ROWS 9
#define COLS 9
#define NUMBER_OF_MINES 10

void board_clear(Cell board[ROWS][COLS]);
// Initialize the board with NUMBER_OF_MINES mines, and guarantee the first
// click is safe.
void board_init(Cell board[ROWS][COLS], int safe_row, int safe_col);
// Reveal the cell when clicked the mouse left botton
bool reveal_cell(Cell board[ROWS][COLS], int hover_row, int hover_col);
// Set the flag when clicked the mouse right botton
void set_flag(Cell board[ROWS][COLS], int hover_row, int hover_col);
// Reveal all mines after the game over.
void reveal_all_mines(Cell board[ROWS][COLS]);
// Check if the game over.
bool board_is_won(Cell board[ROWS][COLS]);

#endif // !BOARD_H
