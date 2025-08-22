#include "text.h"
#include "platform_zx.h"
#include "config.h"

/* =========================================================================
   Text helpers (8x8 font on ZX ULA)
   -------------------------------------------------------------------------
   WHAT:
     - Renders fixed-width 8x8 glyphs aligned to the ULA character grid.
     - Applies attribute colours (ink/paper/bright) behind the text rows.

   HOW:
     - A global attribute (g_text_attr) defines colours.
     - TEXT_FONT8 is a global pointer to a 96*8 table (ASCII 32..127).
       You must set it once via text_use_font8x8() before drawing.
     - text_* functions paint attributes, then call zx_puts8()
       to actually draw the glyphs.

   WHEN TO USE:
     - Menu titles, HUD/status lines, labels. Anything that lives on the 8x8 grid.
   ========================================================================= */

/* Global text attribute (C89 init: no designated/compound literals) */
static Attr g_text_attr = { ZX_WHITE, ZX_BLACK, 1, 0 };

void zx_putc8(uint8_t char_col, uint8_t char_row, char c);
void zx_puts8(uint8_t char_col, uint8_t char_row, const char *s);

/* Active 8×8 font table (96*8 bytes). App sets this once at startup. */
const uint8_t *TEXT_FONT8 = 0;

/* -------------------------------------------------------------------------
   text_use_font8x8
   Select the active 8×8 font used by all text drawing.

   WHAT IT DOES:
     Stores a pointer to a 96×8‑byte table (ASCII 32..127). Each glyph is
     8 bytes top‑to‑bottom; bit 7 (MSB) is the leftmost pixel in the row.

   WHEN TO CALL:
     Call once at startup (or whenever you switch fonts) *before* any
     text_at/text_at_pixels/text_status call.
   ------------------------------------------------------------------------- */
void text_use_font8x8(const uint8_t *font)
{
    TEXT_FONT8 = font;
}

/* -------------------------------------------------------------------------
   text_set_colors
   Set the current text colours used by text_status/text_at/... helpers.

   PARAMS:
     ink   : ZX colour 0..7
     paper : ZX colour 0..7
     bright: 0/1 (BRIGHT bit)

   WHY:
     Keeps colour state in one place; text drawing stays simple.
   ------------------------------------------------------------------------- */
void text_set_colors(uint8_t ink, uint8_t paper, uint8_t bright)
{
    g_text_attr.ink    = ink;
    g_text_attr.paper  = paper;
    g_text_attr.bright = bright ? 1u : 0u;
    g_text_attr.flash  = 0; /* UI text usually keeps FLASH off */
}

/* -------------------------------------------------------------------------
   text_status
   Draw a one-line status message at the bottom attribute row.

   HOW IT WORKS:
     - Paint the entire bottom attribute row with the current colours.
     - Render the string starting at column 0 using zx_puts8().

   WHEN TO USE:
     - Status bars, prompts; anything you want fixed to the last row.
   ------------------------------------------------------------------------- */
void text_status(const char *s)
{
    uint8_t attr = MAKE_ATTR_FROM(g_text_attr);
    zx_fill_attr_rect(0, STATUS_LINE_ROW, ZX_ATTR_COLS, 1, attr);
    zx_puts8(0, STATUS_LINE_ROW, s);
}

/* -------------------------------------------------------------------------
   text_at
   Draw a string at a character cell position (char_col, char_row).

   HOW IT WORKS:
     - Fills that row's attributes from char_col to the right edge
       (cheap, no strlen required; typical for menu lines).
     - Renders the string with zx_puts8().

   WHEN TO USE:
     - Menu items, labels, any 8×8-aligned UI text.
   ------------------------------------------------------------------------- */
void text_at(uint8_t char_col, uint8_t char_row, const char *s)
{
    uint8_t attr = MAKE_ATTR_FROM(g_text_attr);
    zx_fill_attr_rect(char_col, char_row,
                      (uint8_t)(ZX_ATTR_COLS - char_col), 1, attr);
    zx_puts8(char_col, char_row, s);
}

/* -------------------------------------------------------------------------
   text_at_pixels
   Draw a string at a pixel position (x,y) by converting to character cells.

   HOW IT WORKS:
     - Converts (x,y)→(col,row) via division by 8 and forwards to text_at().

   WHEN TO USE:
     - If you computed pixel positions from the grid and want to label cells.
   ------------------------------------------------------------------------- */
void text_at_pixels(uint8_t x, uint8_t y, const char *s)
{
    uint8_t col = (uint8_t)(x / 8u);
    uint8_t row = (uint8_t)(y / 8u);
    text_at(col, row, s);
}


/* -------------------------------------------------------------------------
   zx_putc8
   Render one ASCII character using the active 8×8 font (TEXT_FONT8).

   What it does
   ------------
   - Looks up the glyph for ASCII 32..127 in the global TEXT_FONT8 table.
   - Writes 8 bytes (top line first) straight into the ULA bitmap, one byte
     per scanline. Bit 7 (MSB) is the leftmost pixel; a 1-bit draws ink.

   How it works
   ------------
   dst = zx_pixaddr(char_col*8, char_row*8)
   for i in 0..7:
     *dst = glyph[i]
     dst += 256 (next physical scanline in ZX layout)

   When to use
   -----------
   - All 8×8-aligned UI text. Set the font once with text_use_font8x8().

   Requirements
   ------------
   - TEXT_FONT8 must be non-NULL and point to a 96*8 table (ASCII 32..127).
   ------------------------------------------------------------------------- */
void zx_putc8(uint8_t char_col, uint8_t char_row, char c)
{
    extern const uint8_t *TEXT_FONT8; /* provided by text.c */
    const uint8_t *glyph;
    unsigned char uc = (unsigned char)c;
    uint8_t *dst;
    uint8_t i;

    if (!TEXT_FONT8) return;            /* no font: draw nothing */
    if (uc < 32 || uc > 127) uc = 32;   /* clamp to space */

    glyph = TEXT_FONT8 + (uint16_t)(uc - 32u) * 8u;
    dst   = zx_pixaddr((uint8_t)(char_col * 8u), (uint8_t)(char_row * 8u));

    for (i = 0; i < 8u; ++i) {
        *dst = glyph[i];   /* 1-bit = ink, 0 = paper; no inversion */
        dst += 256u;       /* next physical scanline in ZX bitmap */
    }
}

/* Render NUL-terminated string at (char_col,char_row). */
void zx_puts8(uint8_t char_col, uint8_t char_row, const char *s)
{
    uint8_t x = char_col;
    while (*s) {
        zx_putc8(x, char_row, *s++);
        ++x;
    }
}
