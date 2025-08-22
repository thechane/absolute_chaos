#include "screen_setup.h"
#include "../engine/config.h"
#include "../engine/border.h"
#include "../engine/text.h"

void setup_enter(void) {
    BorderTheme theme = { ZX_CYAN, ZX_BLUE, ZX_BLACK, 1 };
    border_draw(theme, 0); /* key_bloop */
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(4, 5, "SETUP: ENTER WIZARDS");
    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_status("SETUP SCREEN (stub)");
}

void setup_update(void) {
    /* TODO: input */
}