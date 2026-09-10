#pragma once
#include "arcade.h"
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
