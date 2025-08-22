#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

typedef enum {
  SCR_OPENING=0,
  SCR_SETUP,
  SCR_MENU,
  SCR_SPELLBOOK,
  SCR_GAME
} ScreenId;

void screen_init(void);
void screen_switch(ScreenId id);
void screen_update(void);  // call per frame or per step

#endif