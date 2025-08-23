#include "screen_opening.h"
#include "engine/audio.h"
#include "engine/border.h"
#include "engine/text.h"
#include "engine/grid.h"
#include "engine/screen.h"
#include "engine/config.h"
#include "engine/audio.h"

static Grid g;

void opening_enter(void) {
  BorderTheme theme = { ZX_CYAN, ZX_BLUE, ZX_BLACK, 1 };
  border_draw(theme, PAT_VSTRIPE_MED, PAT_VSTRIPE_THIN, 0);   // FIXED
  grid_from_border(&g, GRID_COLS_DEFAULT, GRID_ROWS_DEFAULT, BLOCK_W, BLOCK_H);

  text_set_colors(ZX_YELLOW, ZX_BLACK, 1);
  text_status("ABSOLUTE CHAOS - HOW MANY WIZARDS (2-8)?");

  // Example: title centered inside border
  text_set_colors(ZX_WHITE, ZX_BLACK, 1);
  text_at(6, 4, "ABSOLUTE CHAOS");

  text_set_colors(ZX_GREEN, ZX_BLACK, 1);
  text_at(4, 6, "Enter number of wizards...");
}

void opening_update(void) {
  // TODO: read keys; when confirmed:
  // screen_switch(SCR_SETUP);
}