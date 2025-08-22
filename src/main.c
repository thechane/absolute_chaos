#include "engine/config.h"
#include "engine/text.h"
#include "engine/border.h"
#include "engine/grid.h"
#include "engine/utils.h"
#include "audio/sfx.h"
#include "assets/assets.h"

extern const uint8_t FONT8X8_ZXLIKE[96*8];
extern const uint8_t *TEXT_FONT8;


static void demo_run(void)
{
    Grid g;
    BorderTheme theme;

    /* hook the font once */
    TEXT_FONT8 = FONT8X8_ZXLIKE;

    /* 1) initial border + title + status, with SFX index 0 (see chaos_sfx.c) */
    theme.primary_ink   = ZX_YELLOW;
    theme.secondary_ink = ZX_MAGENTA;
    theme.paper         = ZX_BLACK;
    theme.bright        = 1;
    border_draw(theme, 0); /* play "key_bloop" */

    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(6, 4, "ABSOLUTE CHAOS");

    text_set_colors(ZX_GREEN, ZX_BLACK, 1);
    text_status("DEMO: BORDER + TEXT + GRID");

    wait_frames(50);

    /* 2) build a 16x10 grid centered inside the border and print cell labels */
    grid_from_border(&g, GRID_COLS_DEFAULT, GRID_ROWS_DEFAULT, BLOCK_W, BLOCK_H);

    /* show a few grid anchor labels (char coords) */
    {
        /* top-left cell label */
        uint8_t px = 0, py = 0;
        grid_cell_to_pixel(&g, 0, 0, &px, &py);
        text_set_colors(ZX_CYAN, ZX_BLACK, 1);
        text_at_pixels(px, py, "0,0");

        /* center cell label */
        grid_cell_to_pixel(&g, (uint8_t)(g.cols/2), (uint8_t)(g.rows/2), &px, &py);
        text_set_colors(ZX_WHITE, ZX_BLACK, 1);
        text_at_pixels(px, py, "C");

        /* bottom-right cell label */
        grid_cell_to_pixel(&g, (uint8_t)(g.cols-1), (uint8_t)(g.rows-1), &px, &py);
        text_set_colors(ZX_YELLOW, ZX_BLACK, 1);
        text_at_pixels(px, py, "X");
    }

    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_status("GRID: 16 x 10   BLOCK: 16x16");

    wait_frames(50);

    /* 3) live recolor the border to show theme swapping (no redraw of pixels) */
    theme.primary_ink   = ZX_GREEN;
    theme.secondary_ink = ZX_BLUE;
    border_recolor(theme);

    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_status("BORDER RECOLORED (GREEN/BLUE)");

    wait_frames(50);

    /* 4) show a menu-style block of text to prove positioning/attributes */
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(4, 6, "1 EXAMINE SPELL");
    text_at(4, 7, "2 SELECT  SPELL");
    text_at(4, 8, "3 EXAMINE BOARD");
    text_at(4, 9, "4 DONE");

    text_set_colors(ZX_YELLOW, ZX_BLACK, 1);
    text_status("DEMO COMPLETE");

    /* idle loop so you can admire it in CSpect */
    for (;;) { wait_frames(50); }
}

int main(void)
{
    demo_run();
    return 0;
}
