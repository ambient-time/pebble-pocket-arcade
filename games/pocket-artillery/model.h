#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
// Round screens: put the tank line on the widest chord, at the centre, so both
// tanks clear the rim and the shot's arc rises into the dome above.
#define UI_CHALK_SCALE 100
#define UI_ROUND_FIELD_Y(h) ((h) / 2 - 101 * UI_FIELD_SCALE / 100)
#define GAME_HUD_COMPACT 1
// The Time Round's wider middle chord fits "SELECT: angle/power" on one line.
#define GAME_RULES_COMPACT                                                     \
  PBL_IF_ROUND_ELSE(                                                           \
      "Destroy the tank.\nUP/DOWN adjust.\nSELECT: angle/power\nHold SELECT: fire", \
      "Destroy the tank.\nUP/DOWN adjust.\nSELECT swaps angle\nand power.\nHold SELECT: fire")
enum {
  ANGLE,
  POWER,
  MODE,
  PHASE,
  SHOOTER,
  HP,
  ENEMY_HP,
  SHOT_X,
  SHOT_Y,
  VX,
  VY,
  WIND,
  DELAY,
  BLAST_X,
  BLAST_Y,
  BLAST_AGE,
  SHOTS,
  CPU_ANGLE,
  CPU_POWER,
  TERRAIN = 40
};
void artillery_launch(Game *, int, int, int);
void artillery_impact(Game *, int, int);
void artillery_solution(const Game *, int, int *, int *);
int artillery_y(const Game *, int);
