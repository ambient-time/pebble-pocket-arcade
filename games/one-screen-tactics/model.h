#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
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
