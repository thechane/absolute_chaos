#include "screen.h"
#include "screens/screen_opening.h"
#include "screens/screen_setup.h"
#include "screens/screen_menu.h"
#include "screens/screen_spellbook.h"
#include "screens/screen_game.h"

static ScreenId current;

void screen_init(void) { current = SCR_OPENING; opening_enter(); }
void screen_switch(ScreenId id) {
  current = id;
  switch (id) {
    case SCR_OPENING:   opening_enter(); break;
    case SCR_SETUP:     setup_enter(); break;
    case SCR_MENU:      menu_enter(); break;
    case SCR_SPELLBOOK: spellbook_enter(); break;
    case SCR_GAME:      game_enter(); break;
  }
}
void screen_update(void) {
  switch (current) {
    case SCR_OPENING:   opening_update(); break;
    case SCR_SETUP:     setup_update(); break;
    case SCR_MENU:      menu_update(); break;
    case SCR_SPELLBOOK: spellbook_update(); break;
    case SCR_GAME:      game_update(); break;
  }
}