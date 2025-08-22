#ifndef GRID_H
#define GRID_H
#include <stdint.h>

typedef struct {
  uint8_t cols, rows;
  uint8_t block_w, block_h;   // in pixels
  uint8_t origin_x, origin_y; // pixel top-left inside border
} Grid;

void grid_init(Grid *g, uint8_t cols, uint8_t rows, uint8_t block_w, uint8_t block_h);
void grid_from_border(Grid *g, uint8_t cols, uint8_t rows, uint8_t block_w, uint8_t block_h);
void grid_cell_to_pixel(const Grid *g, uint8_t cx, uint8_t cy, uint8_t *x, uint8_t *y);

#endif