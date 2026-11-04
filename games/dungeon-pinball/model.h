#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 120 // Keep the complete table border and drain in view.
// The table is wider than tall (176 x 144 world units), so 144-pixel
// rectangles are width-bound: the shell's 81% puts every wall on screen.
// Time Round: 74% keeps the table's corners inside the rim and leaves the
// top and bottom chords wide enough for the two caption lines.
#define UI_CHALK_SCALE 74
#define GAME_HUD_COMPACT 1
// No touchscreen on the four compact watches, so no screen-side hint.
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
