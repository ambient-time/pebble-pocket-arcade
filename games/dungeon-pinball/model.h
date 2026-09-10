#pragma once
#include "arcade.h"
enum {
  BALL_X,
  BALL_Y,
  BALL_VX,
  BALL_VY,
  LIVES,
  LEFT,
  RIGHT,
  WAITING,
  BOSS,
  COOLDOWN,
  TOUCH_SIDE,
  STUCK,
  LEFT_AGE,
  RIGHT_AGE,
  BUMPERS = 40
};
void pinball_launch(Game *);
