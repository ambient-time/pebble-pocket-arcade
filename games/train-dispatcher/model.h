#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
// Time Round: the main line sits on the widest chord and every station, the
// loop and the score lines stay inside the rim (the C station is the farthest
// point, 103 field units from the centre).
#define UI_CHALK_SCALE 80
#define GAME_HUD_COMPACT 1
// No touchscreen on the compact watches, so no "Or tap a junction."
#define GAME_RULES_COMPACT \
  "Deliver 18 trains.\nThree misses: over.\nUP/DOWN: switch\nSELECT: change\nOccupied = locked."
enum {
  SW0,
  SW1,
  SW2,
  FOCUS,
  DELIVERED,
  STRIKES,
  SPAWN,
  NEXT,
  COUNT,
  TRAINS = 30
};
#define TRAIN_WORDS 5
void train_position(const Game *, int, int *, int *);
bool train_locked(const Game *, int);
void train_auto(Game *);
