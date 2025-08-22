#include "grid.h"
#include "border.h"
#include "config.h"

/* =========================================================================
   Grid utilities
   -------------------------------------------------------------------------
   A Grid defines a logical board made of fixed-size blocks (tiles).  Each
   block is BLOCK_W x BLOCK_H pixels (passed in at init), and the whole grid
   is cols x rows blocks.  The grid has an origin (origin_x, origin_y) in
   pixels, which is the pixel position of cell (0,0).

   Typical flow:
     - Call grid_from_border(...) to size/center a grid inside the decorative
       border region.
     - Convert cell coordinates to pixels with grid_cell_to_pixel(...).
   ========================================================================= */

/* -------------------------------------------------------------------------
   grid_init
   Initialize a Grid with dimensions and block size.  Origin is set to (0,0).
   Use this if you want to place the grid manually later.

   Params:
     g     : pointer to Grid
     cols  : number of columns (cells)
     rows  : number of rows (cells)
     bw,bh : width/height of a cell in pixels
   ------------------------------------------------------------------------- */
void grid_init(Grid *g, uint8_t cols, uint8_t rows, uint8_t bw, uint8_t bh)
{
    g->cols     = cols;
    g->rows     = rows;
    g->block_w  = bw;
    g->block_h  = bh;
    g->origin_x = 0;
    g->origin_y = 0;
}

/* -------------------------------------------------------------------------
   grid_from_border
   Initialize a Grid and center it inside the current border's inner area.
   The origin is chosen so the grid sits centered; if the grid is larger
   than the inner area in a dimension, it is aligned to the top/left (no
   negative origin or unsigned underflow).

   Params:
     g     : pointer to Grid
     cols  : number of columns (cells)
     rows  : number of rows (cells)
     bw,bh : width/height of a cell in pixels

   How it works:
     - Queries border_get_inner() to get the interior rectangle (x,y,w,h).
     - Computes the pixel span the grid would occupy (cols*bw, rows*bh).
     - If the grid span fits, centers it; otherwise clamps to the interior
       top/left.
   ------------------------------------------------------------------------- */
void grid_from_border(Grid *g, uint8_t cols, uint8_t rows, uint8_t bw, uint8_t bh)
{
    uint8_t x, y, w, h;
    uint16_t span_w, span_h;

    border_get_inner(&x, &y, &w, &h);
    grid_init(g, cols, rows, bw, bh);

    /* Compute desired pixel span using wider ints to avoid overflow. */
    span_w = (uint16_t)cols * (uint16_t)bw;
    span_h = (uint16_t)rows * (uint16_t)bh;

    /* Center if it fits, else align to top/left of the inner rect. */
    if (span_w < (uint16_t)w) {
        g->origin_x = (uint8_t)(x + (uint8_t)((w - (uint8_t)span_w) / 2));
    } else {
        g->origin_x = x;
    }

    if (span_h < (uint16_t)h) {
        g->origin_y = (uint8_t)(y + (uint8_t)((h - (uint8_t)span_h) / 2));
    } else {
        g->origin_y = y;
    }
}

/* -------------------------------------------------------------------------
   grid_cell_to_pixel
   Convert a grid cell coordinate (cx, cy) into a pixel coordinate inside
   the screen.  The returned pixel is the top-left of that cell.

   Params:
     g     : pointer to Grid
     cx,cy : cell coordinates (0..cols-1, 0..rows-1)
     *x,*y : outputs (any may be NULL)

   Usage:
     uint8_t px, py;
     grid_cell_to_pixel(&grid, 5, 3, &px, &py);
     // blit your sprite at (px, py)
   ------------------------------------------------------------------------- */
/* src/engine/grid.c */
void grid_cell_to_pixel(const Grid *g, uint8_t cx, uint8_t cy, uint8_t *x, uint8_t *y)
{
    uint16_t px = (uint16_t)g->origin_x + (uint16_t)cx * (uint16_t)g->block_w;
    uint16_t py = (uint16_t)g->origin_y + (uint16_t)cy * (uint16_t)g->block_h;

    /* clamp so 8×8 text/sprites won’t spill past the bitmap */
    if (px > (uint16_t)(ZX_SCREEN_W - 8)) px = (uint16_t)(ZX_SCREEN_W - 8);
    if (py > (uint16_t)(ZX_SCREEN_H - 8)) py = (uint16_t)(ZX_SCREEN_H - 8);

    if (x) *x = (uint8_t)px;
    if (y) *y = (uint8_t)py;
}
