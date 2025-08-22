#ifndef CONFIG_H
#define CONFIG_H

/* ------------------------------------------------------------------
   ZX Spectrum screen layout constants
   ------------------------------------------------------------------ */

/* Classic 256x192 pixel screen, 32x24 attribute cells */
#define ZX_SCREEN_W       256
#define ZX_SCREEN_H       192
#define ZX_ATTR_COLS      32    /* 256/8 */
#define ZX_ATTR_ROWS      24    /* 192/8 */

/* Typed base addresses (as pointers) */
#define ZX_PIX_BASE   ((uint8_t *)0x4000)  /* pixel bitmap start */
#define ZX_ATTR_BASE  ((uint8_t *)0x5800)  /* attribute table start */

/* ------------------------------------------------------------------
   Game grid / layout
   ------------------------------------------------------------------ */

/* Default block size (same as your sprite tiles) */
#ifndef BLOCK_W
#define BLOCK_W 16
#endif
#ifndef BLOCK_H
#define BLOCK_H 16
#endif

/* Default grid size (can be changed at runtime) */
#define GRID_COLS_DEFAULT 16
#define GRID_ROWS_DEFAULT 10

/* Border thickness (in pixels, drawn inside screen) */
#define BORDER_THICKNESS_PX 16   /* two 8px rings; tweakable */

/* Status line at bottom of screen (in attribute rows) */
#define STATUS_LINE_CHARS   32
#define STATUS_LINE_ROW     23

/* ------------------------------------------------------------------
   ZX colours (0..7)
   ------------------------------------------------------------------ */
enum zx_color {
    ZX_BLACK   = 0,
    ZX_BLUE    = 1,
    ZX_RED     = 2,
    ZX_MAGENTA = 3,
    ZX_GREEN   = 4,
    ZX_CYAN    = 5,
    ZX_YELLOW  = 6,
    ZX_WHITE   = 7
};

/* Optional typedef if you want a type alias */
typedef enum zx_color zx_color_t;

#endif /* CONFIG_H */