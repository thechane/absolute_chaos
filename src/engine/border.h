#ifndef BORDER_H
#define BORDER_H
#include <stdint.h>
#include <stdbool.h>
#include "types.h"

typedef struct {
  uint8_t primary_ink;   // inner ring / details
  uint8_t secondary_ink; // outer ring
  uint8_t paper;         // background behind the frame
  bool    bright;
} BorderTheme;

// Initialize & draw full border, plays the provided sound id (may be 0=none)
void border_draw(BorderTheme theme, uint8_t sfx_id);

// Update colours only (instant recolor via attributes)
void border_recolor(BorderTheme theme);

// Returns interior pixel rect (x,y,w,h) inside the border for layout
void border_get_inner(uint8_t *x, uint8_t *y, uint8_t *w, uint8_t *h);

#endif