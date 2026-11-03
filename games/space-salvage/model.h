#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 115 // The entire flight field must remain visible.
// Time Round: the same rule, so the field's corners meet the rim.
#define UI_CHALK_SCALE 79
#define GAME_HUD_COMPACT 1
#define GAME_RULES_COMPACT \
  "Bank 5 crates; exit.\nAvoid rocks. 150s.\nUP/DOWN turn.\nSELECT thrusts."
enum {
  X,
  Y,
  VX,
  VY,
  ANGLE,
  HULL,
  CARRY,
  BANKED,
  DEADLINE,
  SHIELD,
  TARGET_X,
  TARGET_Y,
  AUTO,
  FX,
  BEAM_X,
  BEAM_Y,
  CARGO = 40,
  ROCKS = 100
};
void salvage_target(Game *, int, int);
