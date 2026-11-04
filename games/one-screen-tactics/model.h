#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
// Pebble Time, Pebble 2, Pebble 2 Duo and the Time Round draw the board in
// pixels (draw.c). These scales only size the shell's field so the bottom
// caption sits under the board: y 144 on 144x168, y 146 on the Time Round.
#define UI_SMALL_SCALE 82
#define UI_CHALK_SCALE 77
#define GAME_HUD_COMPACT 1
#define GAME_RESULT_COMPACT 1
// The title takes two lines on every compact screen (the Time Round's rim
// would cut a one-line title), so the rules start one line lower.
#define GAME_NAME_COMPACT PBL_IF_ROUND_ELSE("One-Screen\nTactics", game_name)
#define GAME_RULES_COMPACT \
  "\nGuard B for 5 turns.\nShove foes into X.\nUP/DOWN: move\nSELECT: act"
enum {
  PX,
  PY,
  HEALTH,
  BEACON_X,
  BEACON_Y,
  INTEGRITY,
  ROUND,
  CHOICE,
  BOARD,
  ENEMIES = 20,
  HAZARDS = 50,
  INTENTS = 70
};
void tactics_act(Game *, int);
