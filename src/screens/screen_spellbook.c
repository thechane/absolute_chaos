#include "screen_spellbook.h"
#include "engine/config.h"
#include "engine/border.h"
#include "engine/text.h"

void spellbook_enter(void) {
    BorderTheme theme = { ZX_GREEN, ZX_MAGENTA, ZX_BLACK, 1 };
    border_draw(theme, PAT_SOLID, PAT_CHECKER_A, 0);   // FIXED
    text_set_colors(ZX_WHITE, ZX_BLACK, 1);
    text_at(4, 5, "SPELL BOOK");
    text_set_colors(ZX_YELLOW, ZX_BLACK, 1);
    text_status("SPELLBOOK SCREEN (stub)");
}

void spellbook_update(void) {
    /* TODO */
}