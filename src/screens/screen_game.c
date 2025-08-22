#include "screen_game.h"
#include "../engine/config.h"
#include "../engine/border.h"
#include "../engine/text.h"

void game_enter(void) {
    BorderTheme theme = { ZX_YELLOW, ZX_RED, ZX_BLACK, 1 };
    border_draw(theme, 0);
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(6, 5, "GAME (DEMO)");
    text_set_colors(ZX_CYAN, ZX_BLACK, 1);
    text_status("GAME SCREEN (stub)");
}

void game_update(void) {
    /* TODO */
}