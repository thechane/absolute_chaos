#include "engine/config.h"
#include "engine/platform_zx.h"
#include "engine/border.h"
#include "engine/text.h"
#include "engine/grid.h"
#include "engine/utils.h"
#include "audio/sfx.h"
#include "assets/assets.h"
#include "assets/font8x8.h" 
#include <stdint.h>

/* font provided by your assets (ASCII 32..127, 96*8 bytes) */
extern const uint8_t FONT8X8_ZXLIKE[96*8];
extern const uint8_t *TEXT_FONT8;


static void demo_grid_labels(const Grid *g)
{
    uint8_t px, py;

    /* top-left cell (0,0) */
    grid_cell_to_pixel(g, 0u, 0u, &px, &py);
    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_at_pixels(px, py, "0,0");

    /* center cell */
    grid_cell_to_pixel(g, (uint8_t)(g->cols/2u), (uint8_t)(g->rows/2u), &px, &py);
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at_pixels(px, py, "C");

    /* bottom-right cell (cols-1, rows-1) */
    grid_cell_to_pixel(g, (uint8_t)(g->cols-1u), (uint8_t)(g->rows-1u), &px, &py);
    text_set_colors(ZX_YELLOW, ZX_BLACK, 1);
    text_at_pixels(px, py, "X");
}

int gameloop(void)
{
    BorderTheme theme;
    Grid grid;

    /* -------------- select the 8×8 ZX‑like font -------------- */
    text_use_font8x8(FONT8X8_ZXLIKE);     /* <= key line */

    /* -------------- draw border / ui text as before ----------- */
    theme.primary_ink   = ZX_CYAN;
    theme.secondary_ink = ZX_BLUE;
    theme.paper         = ZX_BLACK;
    theme.bright        = 1;
    border_draw(theme, PAT_VSTRIPE_MED, PAT_VSTRIPE_THIN, 0);

    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(6u, 3u, "ABSOLUTE CHAOS");

    text_set_colors(ZX_GREEN, ZX_BLACK, 1);
    text_status("DEMO: BORDER + TEXT + GRID (VSTRIPES)");

    wait_frames(60);

    grid_from_border(&grid, GRID_COLS_DEFAULT, GRID_ROWS_DEFAULT, BLOCK_W, BLOCK_H);
    demo_grid_labels(&grid);

    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_status("GRID 16x10  BLOCK 16x16  (CENTERED)");

    wait_frames(60);

    theme.primary_ink   = ZX_YELLOW;
    theme.secondary_ink = ZX_RED;
    border_recolor(theme);

    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_status("BORDER RECOLORED: INNER=YELLOW OUTER=RED");

    for (;;) wait_frames(50);
}