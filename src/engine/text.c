#include "text.h"
#include "platform_zx.h"
#include "config.h"

static Attr g_text_attr = { .ink = ZX_WHITE, .paper = ZX_BLACK, .bright = true, .flash=false };
const uint8_t *TEXT_FONT8 = 0; // set in main to your font table

void text_set_colors(uint8_t ink, uint8_t paper, bool bright) {
  g_text_attr.ink = ink; g_text_attr.paper = paper; g_text_attr.bright = bright;
}

void text_status(const char *s) {
  // attr line at bottom, set color, then write text
  uint8_t attr = make_attr(g_text_attr);
  zx_fill_attr_rect(0, STATUS_LINE_ROW, ZX_ATTR_COLS, 1, attr);
  zx_puts8(0, STATUS_LINE_ROW, s, TEXT_FONT8);
}

void text_at(uint8_t char_col, uint8_t char_row, const char *s) {
  uint8_t attr = make_attr(g_text_attr);
  // naive: set attrs over the length we write (max 32)
  zx_fill_attr_rect(char_col, char_row, ZX_ATTR_COLS - char_col, 1, attr);
  zx_puts8(char_col, char_row, s, TEXT_FONT8);
}

void text_at_pixels(uint8_t x, uint8_t y, const char *s) {
  // y must be multiple of 8 to line up attributes cleanly (menu/forms)
  if (TEXT_FONT8 == 0) return;
  uint8_t col = x/8, row = y/8;
  text_at(col, row, s);
}