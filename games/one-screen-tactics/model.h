#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
// The 5x5 board is square, so it fits both shapes. On 144x168 it takes the
// largest size that leaves a caption row above and below (118 px). On the Time
// Round it stays centred with its corners well inside the rim and both
// captions inside the circle's chord (109 px).
#define UI_SMALL_SCALE 84
#define UI_CHALK_SCALE 78
#define GAME_HUD_COMPACT 1
// The title takes two lines on these screens (game.c), so the rules start one
// line lower. No touch line on watches without a touchscreen.
#define GAME_RULES_COMPACT                                                     \
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
