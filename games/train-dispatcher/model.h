#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
// Time Round: the whole network stays inside the rim. At 80% station C's
// corner clears the rim, and the field sits 2 px below centre so station A
// clears the score line.
#define UI_CHALK_SCALE 80
#if defined(PBL_PLATFORM_CHALK)
#define UI_ROUND_FIELD_Y(h) ((h) / 2 - 56)
#endif
#define GAME_HUD_COMPACT 1
// Buttons only on the four compact watches, so no tap line.
#define GAME_RULES_COMPACT                                                     \
  PBL_IF_ROUND_ELSE("Deliver 18 trains.\n3 misses: over.\nUP/DOWN: junction\n" \
                    "SELECT: switch it\nOccupied = locked",                    \
                    "Deliver 18 trains.\nThree misses: over.\nUP/DOWN: junction\n" \
                    "SELECT: switch it\nOccupied = locked.")
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
