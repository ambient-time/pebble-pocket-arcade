#pragma once
#include "arcade.h"
enum {
  SW0,
  SW1,
  SW2,
  FOCUS,
  DELIVERED,
  STRIKES,
  SPAWN,
  NEXT,
  COUNT,
  TRAINS = 30
};
#define TRAIN_WORDS 5
void train_position(const Game *, int, int *, int *);
bool train_locked(const Game *, int);
void train_auto(Game *);
