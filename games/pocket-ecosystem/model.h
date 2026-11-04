#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 115 // Preserve the corner habitat cells on the circle.
// Time Round: the default 79% keeps the grid's corners inside the rim; the
// tool legend is inset (draw.c).
#define GAME_HUD_COMPACT 1
// No touch on the compact watches, so the rules drop "Or tap tool + cell."
// The Time Round has room for four lines; the moving cursor shows UP/DOWN.
#define GAME_RULES_COMPACT                                                     \
  PBL_IF_ROUND_ELSE(                                                           \
      "Plants + 3 grazers\n+ a hunter: 60s.\nSELECT: place\nHold SELECT: tool", \
      "Plants + 3 grazers\n+ a hunter: 60s.\nUP/DOWN: cell\nSELECT: place\nHold SELECT: tool")
enum {
  CURSOR,
  TOOL,
  WATER,
  BUDGET,
  SECONDS,
  BALANCE,
  EXTINCT,
  PLANTS = 30,
  ANIMALS = 80
};
#define COLS 6
#define ROWS 4
#define SLOTS 18
void ecosystem_counts(const Game *, int *, int *, int *);
