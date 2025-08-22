#include "border.h"
#include "platform_zx.h"
#include "config.h"
#include "audio/sfx.h"

// simple 8x8 dot/dither patterns for rings
static const uint8_t PAT_DOT_A[8] = {0x00,0x88,0x00,0x22,0x00,0x88,0x00,0x22};
static const uint8_t PAT_DOT_B[8] = {0x00,0x11,0x00,0x44,0x00,0x11,0x00,0x44};
static const uint8_t PAT_SOLID [8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static void blit_h_strip(uint8_t y, const uint8_t *pat, uint8_t tiles) {
  uint8_t i;
  for (i=0;i<tiles;i++) zx_blit8_aligned(i*8, y, pat);
}
static void blit_v_strip(uint8_t x, const uint8_t *pat, uint8_t tiles) {
  uint8_t i;
  for (i=0;i<tiles;i++) zx_blit8_aligned(x, i*8, pat);
}

void border_get_inner(uint8_t *x, uint8_t *y, uint8_t *w, uint8_t *h) {
  const uint8_t t = BORDER_THICKNESS_PX;
  if (x) *x = t;
  if (y) *y = t;
  if (w) *w = ZX_SCREEN_W - t*2;
  if (h) *h = ZX_SCREEN_H - t*2;
}

static void paint_attr_frame(uint8_t ink, uint8_t paper, bool bright, uint8_t thickness_px) {
  Attr a = { .ink=ink, .paper=paper, .bright=bright, .flash=false };
  uint8_t attr = make_attr(a);

  uint8_t cth = thickness_px/8; // in attr cells
  uint8_t inner_rows = ZX_ATTR_ROWS - 2*cth;

  // Top/Bottom strips
  zx_fill_attr_rect(0, 0, ZX_ATTR_COLS, cth, attr);
  zx_fill_attr_rect(0, ZX_ATTR_ROWS-cth, ZX_ATTR_COLS, cth, attr);
  // Left/Right strips
  zx_fill_attr_rect(0, cth, cth, inner_rows, attr);
  zx_fill_attr_rect(ZX_ATTR_COLS-cth, cth, cth, inner_rows, attr);
}

void border_recolor(BorderTheme theme) {
  // Outer ring = secondary; inner ring = primary.
  // We recolor attributes only (fast), pixels remain as is.
  uint8_t cth = BORDER_THICKNESS_PX/8;
  // Outer
  paint_attr_frame(theme.secondary_ink, theme.paper, theme.bright, BORDER_THICKNESS_PX);
  // Inner ring (one cell inset)
  Attr a = { .ink=theme.primary_ink, .paper=theme.paper, .bright=theme.bright, .flash=false };
  uint8_t attr = make_attr(a);
  zx_fill_attr_rect(cth, cth, ZX_ATTR_COLS-2*cth, 1, attr);                          // top inner
  zx_fill_attr_rect(cth, ZX_ATTR_ROWS-cth-1, ZX_ATTR_COLS-2*cth, 1, attr);           // bottom inner
  zx_fill_attr_rect(cth, cth+1, 1, ZX_ATTR_ROWS-2*cth-2, attr);                       // left inner
  zx_fill_attr_rect(ZX_ATTR_COLS-cth-1, cth+1, 1, ZX_ATTR_ROWS-2*cth-2, attr);        // right inner
}

void border_draw(BorderTheme theme, uint8_t sfx_id) {
  // Clear
  zx_cls_pixels();
  Attr bg = { .ink=theme.paper, .paper=theme.paper, .bright=theme.bright, .flash=false };
  zx_cls_attr(make_attr(bg));

  // Draw pixel rings (two rings of 8px each)
  const uint8_t tiles_w = ZX_SCREEN_W/8;
  const uint8_t tiles_h = ZX_SCREEN_H/8;

  // Outer ring (secondary) – top/bottom/left/right
  uint8_t y;
for (y = 0; y < 8; ++y) {
  blit_h_strip((uint8_t)(0 + y), PAT_DOT_B, (uint8_t)tiles_w);
  blit_h_strip((uint8_t)(ZX_SCREEN_H - 8 + y), PAT_DOT_B, (uint8_t)tiles_w);
}

for (y = 0; y < 8; ++y) {
  blit_h_strip((uint8_t)(8 + y), PAT_DOT_A, (uint8_t)(tiles_w - 2));
  blit_h_strip((uint8_t)(ZX_SCREEN_H - 16 + y), PAT_DOT_A, (uint8_t)(tiles_w - 2));
}

  // Inner ring (primary) – inset 8px

  for (y=0; y<8; ++y) {
    blit_h_strip(8+y, PAT_DOT_A, tiles_w-2);
    blit_h_strip(ZX_SCREEN_H-16+y, PAT_DOT_A, tiles_w-2);
  }

  for (y=16; y<ZX_SCREEN_H-16; y+=8) {
    zx_blit8_aligned(8, y, PAT_DOT_A);
    zx_blit8_aligned(ZX_SCREEN_W-16, y, PAT_DOT_A);
  }

  // Attribute colouring of frame
  border_recolor(theme);

  // Optional: match HW border to secondary ink for authenticity
  zx_out_border(theme.secondary_ink);

  // Pling!
  if (sfx_id) sfx_play_idx(1);
}