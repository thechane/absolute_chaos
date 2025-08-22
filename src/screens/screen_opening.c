#include "screen_opening.h"
#include "../engine/border.h"
#include "../engine/text.h"
#include "../engine/grid.h"
#include "../engine/screen.h"
#include "../audio/sfx.h"
#include "../engine/config.h"
#include "../audio/sfx_ids.h"

static Grid g;

void opening_enter(void) {
  BorderTheme theme = { .primary_ink = ZX_CYAN, .secondary_ink = ZX_BLUE, .paper = ZX_BLACK, .bright=true };
  border_draw(theme, SFX_KEY_BLOOP);

  grid_from_border(&g, GRID_COLS_DEFAULT, GRID_ROWS_DEFAULT, BLOCK_W, BLOCK_H);

  text_set_colors(ZX_YELLOW, ZX_BLACK, true);
  text_status("ABSOLUTE CHAOS - HOW MANY WIZARDS (2-8)?");

  // Example: title centered inside border
  text_set_colors(ZX_WHITE, ZX_BLACK, true);
  text_at(6, 4, "ABSOLUTE CHAOS");

  text_set_colors(ZX_GREEN, ZX_BLACK, true);
  text_at(4, 6, "Enter number of wizards...");
}

void opening_update(void) {
  // TODO: read keys; when confirmed:
  // screen_switch(SCR_SETUP);
}