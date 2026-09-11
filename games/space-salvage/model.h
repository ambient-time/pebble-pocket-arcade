#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 115 // The entire flight field must remain visible.
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
