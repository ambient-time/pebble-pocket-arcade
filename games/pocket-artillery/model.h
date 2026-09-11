#pragma once
#include "arcade.h"
#define UI_ROUND_SCALE 125
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
