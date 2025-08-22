#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include "config.h"
#include "platform_zx.h"

/* =========================================================================
   Text Rendering Interface (8x8 font on ZX ULA)
   -------------------------------------------------------------------------
   - Provides simple routines to render strings using a user-supplied 8x8 font.
   - Each glyph is 8x8 pixels, aligned to the ZX Spectrum's character grid.
   - Text colours are controlled via a global attribute (ink/paper/bright/flash).
   - Attributes are filled automatically behind drawn text.
   ========================================================================= */

/* Global pointer to active font (must be set before drawing).
   Points to a table of 96*8 bytes covering ASCII 32..127. */
extern const uint8_t *TEXT_FONT8;

/* Select the active 8×8 font. Pass FONT8X8_ZXLIKE (or your own). */
void text_use_font8x8(const uint8_t *font);

/* -------------------------------------------------------------------------
   text_set_colors
   Set the global text colour state for subsequent rendering.

   Params:
     ink   : ZX colour index (0..7)
     paper : ZX colour index (0..7)
     bright: non-zero = BRIGHT on, 0 = normal
   ------------------------------------------------------------------------- */
void text_set_colors(uint8_t ink, uint8_t paper, uint8_t bright);

/* -------------------------------------------------------------------------
   text_status
   Draw a one-line status message at the bottom row of the screen.

   Notes:
     - Automatically fills the attribute row with the active colours.
     - Clips naturally at 32 characters.
   ------------------------------------------------------------------------- */
void text_status(const char *s);

/* -------------------------------------------------------------------------
   text_at
   Draw a string at character cell coordinates (char_col, char_row).
   Also paints the attribute cells of that row from char_col to the edge.

   Params:
     char_col : column in characters (0..31)
     char_row : row in characters (0..23)
     s        : NUL-terminated ASCII string
   ------------------------------------------------------------------------- */
void text_at(uint8_t char_col, uint8_t char_row, const char *s);

/* -------------------------------------------------------------------------
   text_at_pixels
   Draw a string at pixel coordinates (x,y), auto-aligned to cell grid.

   Params:
     x,y : pixel coordinates. For best results:
           - x should be a multiple of 8
           - y should be a multiple of 8
   ------------------------------------------------------------------------- */
void text_at_pixels(uint8_t x, uint8_t y, const char *s);

#endif /* TEXT_H */