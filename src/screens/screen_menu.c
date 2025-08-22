#include "screen_menu.h"          /* <-- include your own header to lock prototypes */
#include "engine/config.h"
#include "engine/border.h"
#include "engine/text.h"
#include "audio/sfx.h"

/* 0 = key_bloop, 1..7 other sfx */
void menu_enter(void) {
    BorderTheme theme = { ZX_YELLOW, ZX_MAGENTA, ZX_BLACK, 1 };
    border_draw(theme, 0);
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(4, 5, "1 EXAMINE SPELL");
    text_at(4, 6, "2 SELECT  SPELL");
    text_at(4, 7, "3 EXAMINE BOARD");
    text_at(4, 8, "4 DONE");
    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_status("SELECT OPTION (1-4)");
}

void menu_update(void) {
    /* TODO */
}