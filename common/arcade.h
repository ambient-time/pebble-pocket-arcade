// Luke Steuber. Platform-independent game contract.
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define FIELD_W 176
#define FIELD_H 144
#define GAME_WORDS 900
typedef struct {
  uint32_t seed, ticks, score;
  int16_t status, stage;
  int32_t d[GAME_WORDS];
} Game;
enum {
  ACT_UP,
  ACT_SELECT,
  ACT_DOWN,
  ACT_SPECIAL,
  ACT_RELEASE_UP,
  ACT_RELEASE_DOWN,
  ACT_RELEASE_SELECT
};
extern const char *game_name, *game_rules;
extern const int game_controls;
extern const int game_continuous;
void game_init(Game *, uint32_t);
void game_input(Game *, int);
void game_touch(Game *, int, int, int);
void game_tick(Game *);
bool game_running(const Game *);
bool game_valid(const Game *);
void game_hud(const Game *, char *, size_t, char *, size_t);
static inline uint32_t rnd(Game *g) {
  g->seed = g->seed * 1664525u + 1013904223u;
  return g->seed;
}
static inline int limit(int x, int low, int high) {
  return x < low ? low : x > high ? high : x;
}
