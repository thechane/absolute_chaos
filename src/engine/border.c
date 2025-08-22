#include "border.h"
#include "platform_zx.h"
#include "config.h"
#include "audio/sfx.h"

/* ----- Exposed 8x8 patterns (see header) ----- */
const uint8_t PAT_VSTRIPE_THIN[8]  = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA };
const uint8_t PAT_VSTRIPE_MED [8]  = { 0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC };
const uint8_t PAT_VSTRIPE_THICK[8] = { 0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0 };
const uint8_t PAT_CHECKER_A   [8]  = { 0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55 };
const uint8_t PAT_CHECKER_B   [8]  = { 0xCC,0x33,0xCC,0x33,0xCC,0x33,0xCC,0x33 };
const uint8_t PAT_SOLID       [8]  = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };



/* -------------------------------------------------------------------------
   blit_h_span
   Draw a horizontal run of N 8x8 tiles starting at pixel (x0, y).
   This is used for the top and bottom strips of the rings.

   Params:
     x0    : starting x in pixels (must be multiple of 8)
     y     : y in pixels
     pat   : pointer to 8 bytes of pattern data (MSB is leftmost pixel)
     tiles : number of 8‑pixel tiles to draw

   Usage:
     blit_h_span(8, 16, PAT_DOT_A, 30);
   ------------------------------------------------------------------------- */
static void blit_h_span(uint8_t x0, uint8_t y, const uint8_t *pat, uint8_t tiles)
{
    uint8_t i;
    for (i = 0; i < tiles; ++i) {
        zx_blit8_aligned((uint8_t)(x0 + (uint8_t)(i * 8)), y, pat);
    }
}

/* -------------------------------------------------------------------------
   blit_v_strip
   Draw a vertical run of N 8x8 tiles at pixel x (used for left/right edges).

   Params:
     x     : x in pixels (must be multiple of 8)
     pat   : 8‑byte pattern
     tiles : number of tiles vertically

   Usage:
     blit_v_strip(0, PAT_DOT_B, 22);
   ------------------------------------------------------------------------- */
static void blit_v_strip(uint8_t x, const uint8_t *pat, uint8_t tiles)
{
    uint8_t i;
    for (i = 0; i < tiles; ++i) {
        zx_blit8_aligned(x, (uint8_t)(i * 8), pat);
    }
}

/* -------------------------------------------------------------------------
   border_get_inner
   Returns the interior rectangle (in pixels) that lies inside the full
   decorative border. Use this for centering grids/menus inside the frame.

   Outputs (any pointer may be NULL):
     *x,*y : top‑left of interior area
     *w,*h : width/height of interior area
   ------------------------------------------------------------------------- */
void border_get_inner(uint8_t *x, uint8_t *y, uint8_t *w, uint8_t *h)
{
    const uint8_t t = BORDER_THICKNESS_PX;
    if (x) *x = t;
    if (y) *y = t;
    if (w) *w = (uint8_t)(ZX_SCREEN_W - (uint8_t)(t * 2));
    if (h) *h = (uint8_t)(ZX_SCREEN_H - (uint8_t)(t * 2));
}

/* -------------------------------------------------------------------------
   paint_attr_frame
   Fills the attribute map with a rectangular “frame” of the given thickness.
   This colours both rings at once. Pixel data is not touched.

   Params:
     ink, paper : ZX colour indices (0..7)
     bright     : 0/1 for BRIGHT bit
     thickness  : border thickness in pixels (must be multiple of 8)

   Internals:
     Converts pixel thickness to attribute cells and fills top/bottom and
     left/right strips around the screen.
   ------------------------------------------------------------------------- */
static void paint_attr_frame(uint8_t ink, uint8_t paper, uint8_t bright, uint8_t thickness_px)
{
    Attr a;
    uint8_t attr;
    uint8_t cth;         /* thickness in attribute cells (8 px per cell) */
    uint8_t inner_rows;

    a.ink    = ink;
    a.paper  = paper;
    a.bright = bright;
    a.flash  = 0;
    attr = MAKE_ATTR_FROM(a);

    cth = (uint8_t)(thickness_px / 8);
    inner_rows = (uint8_t)(ZX_ATTR_ROWS - (uint8_t)(2 * cth));

    /* Top / Bottom strips */
    zx_fill_attr_rect(0, 0, ZX_ATTR_COLS, cth, attr);
    zx_fill_attr_rect(0, (uint8_t)(ZX_ATTR_ROWS - cth), ZX_ATTR_COLS, cth, attr);

    /* Left / Right strips */
    zx_fill_attr_rect(0, cth, cth, inner_rows, attr);
    zx_fill_attr_rect((uint8_t)(ZX_ATTR_COLS - cth), cth, cth, inner_rows, attr);
}

/* -------------------------------------------------------------------------
   border_recolor
   Recolours the existing border instantly by touching only attributes.
   Use when you want to change the theme (e.g., success/failure) without
   redrawing any pixel data.

   Params:
     theme : BorderTheme with primary (inner ring), secondary (outer ring),
             paper (background), bright (0/1).
   ------------------------------------------------------------------------- */
void border_recolor(BorderTheme theme)
{
    uint8_t cth = (uint8_t)(BORDER_THICKNESS_PX / 8);
    Attr a;
    uint8_t attr;

    /* Outer ring + paper */
    paint_attr_frame(theme.secondary_ink, theme.paper, theme.bright, BORDER_THICKNESS_PX);

    /* Inner ring (one cell inset on all sides) */
    a.ink    = theme.primary_ink;
    a.paper  = theme.paper;
    a.bright = theme.bright;
    a.flash  = 0;
    attr = MAKE_ATTR_FROM(a);

    zx_fill_attr_rect(cth, cth, (uint8_t)(ZX_ATTR_COLS - (uint8_t)(2 * cth)), 1, attr);                 /* top inner */
    zx_fill_attr_rect(cth, (uint8_t)(ZX_ATTR_ROWS - cth - 1), (uint8_t)(ZX_ATTR_COLS - (uint8_t)(2 * cth)), 1, attr); /* bottom inner */
    zx_fill_attr_rect(cth, (uint8_t)(cth + 1), 1, (uint8_t)(ZX_ATTR_ROWS - (uint8_t)(2 * cth) - 2), attr);            /* left inner */
    zx_fill_attr_rect((uint8_t)(ZX_ATTR_COLS - cth - 1), (uint8_t)(cth + 1), 1, (uint8_t)(ZX_ATTR_ROWS - (uint8_t)(2 * cth) - 2), attr); /* right inner */
}

void border_draw(BorderTheme theme,
                 const uint8_t *pat_outer,
                 const uint8_t *pat_inner,
                 int sfx_index)
{

    /* clear to paper colour */
    {
        Attr bg;
        bg.ink    = theme.paper;
        bg.paper  = theme.paper;
        bg.bright = theme.bright;
        bg.flash  = 0;
        zx_cls_pixels();
        zx_cls_attr(MAKE_ATTR_FROM(bg));
    }

/* ---- Outer ring (secondary) ---- */
{
    uint8_t y, tiles_w = (uint8_t)(ZX_SCREEN_W / 8u);

    for (y = 0u; y < 8u; ++y) {
        blit_h_span(0u, y, pat_outer, tiles_w);                       /* top */
        blit_h_span(0u, (uint8_t)(ZX_SCREEN_H - 8u + y), pat_outer, tiles_w);
    }
    for (y = 8u; y < (uint8_t)(ZX_SCREEN_H - 8u); y += 8u) {
        zx_blit8_aligned(0u, y, pat_outer);                           /* left  */
        zx_blit8_aligned((uint8_t)(ZX_SCREEN_W - 8u), y, pat_outer);  /* right */
    }
}

/* ---- Inner ring (primary) ---- */
{
    uint8_t y, tiles_w = (uint8_t)(ZX_SCREEN_W / 8u);

    for (y = 0u; y < 8u; ++y) {
        blit_h_span(8u, (uint8_t)(8u + y), pat_inner, (uint8_t)(tiles_w - 2u));
        blit_h_span(8u, (uint8_t)(ZX_SCREEN_H - 16u + y),
                    pat_inner, (uint8_t)(tiles_w - 2u));
    }
    for (y = 16u; y < (uint8_t)(ZX_SCREEN_H - 16u); y += 8u) {
        zx_blit8_aligned(8u, y, pat_inner);                           /* left  */
        zx_blit8_aligned((uint8_t)(ZX_SCREEN_W - 16u), y, pat_inner); /* right */
    }
}

    /* colour attributes + hw border */
    border_recolor(theme);
    zx_out_border(theme.secondary_ink);

    /* optional SFX */
    if (sfx_index >= 0) {
        sfx_play_idx((uint16_t)sfx_index);
    }
}