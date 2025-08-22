#include "grid.h"
#include "border.h"
#include "config.h"

void grid_init(Grid *g, uint8_t cols, uint8_t rows, uint8_t bw, uint8_t bh) {
  g->cols = cols; g->rows = rows; g->block_w = bw; g->block_h = bh;
  g->origin_x = 0; g->origin_y = 0;
}
void grid_from_border(Grid *g, uint8_t cols, uint8_t rows, uint8_t bw, uint8_t bh) {
  uint8_t x,y,w,h;
  border_get_inner(&x,&y,&w,&h);
  grid_init(g, cols, rows, bw, bh);
  // Center the grid inside the border
  g->origin_x = x + (uint8_t)((w  - cols*bw)/2);
  g->origin_y = y + (uint8_t)((h  - rows*bh)/2);
}
void grid_cell_to_pixel(const Grid *g, uint8_t cx, uint8_t cy, uint8_t *x, uint8_t *y) {
  if (x) *x = g->origin_x + cx * g->block_w;
  if (y) *y = g->origin_y + cy * g->block_h;
}