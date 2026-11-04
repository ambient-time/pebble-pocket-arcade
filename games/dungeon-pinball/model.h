#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 120 // Keep the complete table border and drain in view.
// 144x168: the table walls (world x 6..170) reach both screen edges, so the
// flippers, skulls and ball are as large as the width allows; the score and
// status lines fit above and below. The table never rotates or crops: the drain
// stays at the bottom and both walls stay visible.
#define UI_SMALL_SCALE 87
// Time Round: 78% keeps the whole table inside the 180 px circle (corners
// 7 px inside the rim) and leaves room above and below it where the circle is
// still wide enough for the chamber line and a 105 px status line.
#define UI_CHALK_SCALE 78
// Time Round: two pixels above centre, so the status line under the table
// clears the rim (its outer letters otherwise touch the circle) while the top
// rail still sits below the chamber line.
#ifdef PBL_PLATFORM_CHALK
#define UI_ROUND_FIELD_Y(h) (((h) - FIELD_H * UI_CHALK_SCALE / 100) / 2 - 2)
#endif
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
