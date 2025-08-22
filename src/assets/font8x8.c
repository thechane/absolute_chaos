#include "assets.h"

// Placeholder ZX-like 8x8 (ASCII 32..127). Replace with your ripped font.
// For brevity here we only include a couple glyphs; fill with your data.
const uint8_t FONT8X8_ZXLIKE[96*8] = {
  // ' ' (space)
  0,0,0,0,0,0,0,0,
  // '!' 
  0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x00,
  // '"' etc...
  // TODO: paste full font table (ZX-ROM style or your own ripped version)
};