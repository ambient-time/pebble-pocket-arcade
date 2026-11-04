#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 120 // Keep the complete table border and drain in view.
// 144x168: the table walls (world x 6..170) reach both screen edges, so the
// flippers, skulls and ball are as large as the width allows; the score and
// status lines fit above and below. The table never rotates or crops: the drain
// stays at the bottom and both walls stay visible.
#define UI_SMALL_SCALE 87
// Time Round: the whole table border fits inside the 180 px circle, with its
// corners about a pixel inside the rim (the Round 2 proportions).
#define UI_CHALK_SCALE 83
#define GAME_HUD_COMPACT 1
#define GAME_RULES_COMPACT                                                     \
  "Clear 3 chambers.\nSkulls unlock boss.\nHit boss 3 times.\nUP/DOWN: flippers\nSELECT launches."
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
