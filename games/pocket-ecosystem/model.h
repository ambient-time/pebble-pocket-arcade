#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 115 // Preserve the corner habitat cells on the circle.
// Time Round: the habitat and tool row stay whole inside the circle, and the
// bottom readout sits on a chord wide enough for its line.
#define UI_CHALK_SCALE 74
#define GAME_HUD_COMPACT 1
// The compact watches have no touchscreen, so the touch line is dropped.
#define GAME_RULES_COMPACT \
  "Plants + 3 grazers\n+ a hunter: 60s.\nUP/DOWN: cell\nSELECT: place\nHold SELECT: tool"
enum {
  CURSOR,
  TOOL,
  WATER,
  BUDGET,
  SECONDS,
  BALANCE,
  EXTINCT,
  PLANTS = 30,
  ANIMALS = 80
};
#define COLS 6
#define ROWS 4
#define SLOTS 18
void ecosystem_counts(const Game *, int *, int *, int *);
