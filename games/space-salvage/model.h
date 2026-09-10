#pragma once
#include "arcade.h"
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
