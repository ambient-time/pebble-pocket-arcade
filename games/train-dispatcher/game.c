#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "Train Dispatcher";
const char *game_rules =
    "Route 18 trains to A/B/C.\nThree mistakes end a shift.\nUP/DOWN choose a "
    "switch.\nSELECT toggles its track.\nOr tap a junction.\nA train on a "
    "switch locks it.";
const int train_paths[9][4] = {
    {3, 72, 48, 72},     {48, 72, 48, 12},    {48, 72, 96, 72},
    {96, 72, 96, 132},   {96, 72, 144, 72},   {144, 72, 171, 26},
    {144, 72, 144, 125}, {144, 125, 24, 125}, {24, 125, 3, 72}};
static int length(int seg) {
  int x = abs(train_paths[seg][2] - train_paths[seg][0]),
      y = abs(train_paths[seg][3] - train_paths[seg][1]);
  return x + y;
}
void train_position(const Game *g, int i, int *x, int *y) {
  int b = TRAINS + i * TRAIN_WORDS, s = g->d[b + 1], pos = g->d[b + 2];
  *x = train_paths[s][0] +
       (train_paths[s][2] - train_paths[s][0]) * pos / (length(s) * 100);
  *y = train_paths[s][1] +
       (train_paths[s][3] - train_paths[s][1]) * pos / (length(s) * 100);
}
bool train_locked(const Game *g, int j) {
  int xx = 48 + j * 48;
  for (int i = 0; i < 8; i++)
    if (g->d[TRAINS + i * TRAIN_WORDS]) {
      int x, y;
      train_position(g, i, &x, &y);
      if (abs(x - xx) + abs(y - 72) < 9)
        return true;
    }
  return false;
}
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[SPAWN] = 15;
  g->d[NEXT] = rnd(g) % 3;
}
static void arrived(Game *g, int b) {
  int s = g->d[b + 1];
  g->d[b + 2] = 0;
  if (s == 0)
    g->d[b + 1] = g->d[SW0] ? 1 : 2;
  else if (s == 2)
    g->d[b + 1] = g->d[SW1] ? 3 : 4;
  else if (s == 4)
    g->d[b + 1] = g->d[SW2] ? 6 : 5;
  else if (s == 6)
    g->d[b + 1] = 7;
  else if (s == 7)
    g->d[b + 1] = 8;
  else if (s == 8)
    g->d[b + 1] = 0;
  else {
    int station = s == 1 ? 0 : s == 3 ? 1 : 2;
    if (g->d[b + 3] == station) {
      g->d[DELIVERED]++;
      g->score += 100;
    } else
      g->d[STRIKES]++;
    g->d[b] = 0;
  }
}
void game_tick(Game *g) {
  if (g->status)
    return;
  g->ticks++;
  g->stage = g->d[DELIVERED] / 6;
  int speed = 55 + g->stage * 12;
  for (int i = 0; i < 8; i++) {
    int b = TRAINS + i * TRAIN_WORDS;
    if (!g->d[b])
      continue;
    g->d[b + 2] += speed;
    if (g->d[b + 2] >= length(g->d[b + 1]) * 100)
      arrived(g, b);
  }
  for (int i = 0; i < 8; i++)
    if (g->d[TRAINS + i * TRAIN_WORDS])
      for (int j = i + 1; j < 8; j++)
        if (g->d[TRAINS + j * TRAIN_WORDS]) {
          int x, y, xx, yy;
          train_position(g, i, &x, &y);
          train_position(g, j, &xx, &yy);
          if (abs(x - xx) + abs(y - yy) < 7) {
            g->d[TRAINS + i * TRAIN_WORDS] = g->d[TRAINS + j * TRAIN_WORDS] = 0;
            g->d[STRIKES]++;
          }
        }
  if (g->d[SPAWN] > 0)
    g->d[SPAWN]--;
  if (!g->d[SPAWN]) {
    bool clear = true;
    int slot = -1;
    for (int i = 0; i < 8; i++) {
      int b = TRAINS + i * TRAIN_WORDS;
      if (!g->d[b]) {
        slot = i;
        continue;
      }
      int x, y;
      train_position(g, i, &x, &y);
      if (abs(x - 3) + abs(y - 72) < 25)
        clear = false;
    }
    if (clear && slot >= 0) {
      int b = TRAINS + slot * TRAIN_WORDS;
      g->d[b] = 1;
      g->d[b + 1] = g->d[b + 2] = 0;
      g->d[b + 3] = g->d[NEXT];
      g->d[NEXT] = rnd(g) % 3;
      g->d[SPAWN] = 150 - g->stage * 20;
      g->d[COUNT]++;
    }
  }
  if (g->d[STRIKES] >= 3)
    g->status = 2;
  else if (g->d[DELIVERED] >= 18)
    g->status = 1;
}
void game_input(Game *g, int a) {
  if (g->status)
    return;
  if (a == ACT_UP)
    g->d[FOCUS] = (g->d[FOCUS] + 2) % 3;
  else if (a == ACT_DOWN)
    g->d[FOCUS] = (g->d[FOCUS] + 1) % 3;
  else if (a == ACT_SELECT && !train_locked(g, g->d[FOCUS]))
    g->d[g->d[FOCUS]] ^= 1;
}
void game_touch(Game *g, int x, int y, int type) {
  if (type != 2 || abs(y - 72) > 27)
    return;
  int nearest = limit((x - 24) / 48, 0, 2);
  if (abs(x - (48 + nearest * 48)) > 25)
    return;
  g->d[FOCUS] = nearest;
  game_input(g, ACT_SELECT);
}
void train_auto(Game *g) {
  for (int junction = 0; junction < 3; junction++) {
    int best = 10000, kind = -1;
    for (int i = 0; i < 8; i++) {
      int b = TRAINS + i * TRAIN_WORDS;
      if (!g->d[b] || g->d[b + 1] != junction * 2)
        continue;
      int remaining = length(junction * 2) * 100 - g->d[b + 2];
      if (remaining < best) {
        best = remaining;
        kind = g->d[b + 3];
      }
    }
    if (kind >= 0 && !train_locked(g, junction))
      g->d[junction] = junction == 0   ? kind == 0
                       : junction == 1 ? kind == 1
                                       : 0;
  }
}
bool game_running(const Game *g) { return !g->status; }
bool game_valid(const Game *g) {
  if (g->status < 0 || g->status > 2 || g->d[FOCUS] < 0 || g->d[FOCUS] > 2 ||
      g->d[STRIKES] < 0 || g->d[STRIKES] > 10 || g->d[DELIVERED] < 0 ||
      g->d[DELIVERED] > 20)
    return false;
  for (int i = 0; i < 8; i++) {
    int b = TRAINS + i * TRAIN_WORDS;
    if (g->d[b + 1] < 0 || g->d[b + 1] > 8 || g->d[b + 2] < 0 ||
        g->d[b + 2] > 12000 || g->d[b + 3] < 0 || g->d[b + 3] > 2)
      return false;
  }
  return true;
}
void game_hud(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, "Delivered %ld/18  X %ld/3", (long)g->d[DELIVERED],
           (long)g->d[STRIKES]);
  snprintf(b, m, "Switch %ld%s  Next %c", (long)g->d[FOCUS] + 1,
           train_locked(g, g->d[FOCUS]) ? " LOCKED" : "",
           'A' + (int)g->d[NEXT]);
}

const int game_controls = 0;

const int game_continuous = 0;

void game_debug(const Game *g, char *out, size_t n) {
  size_t used = 0;
  for (int i = 0; i < 8; i++) {
    int b = TRAINS + i * TRAIN_WORDS;
    if (g->d[b]) {
      int wrote =
          snprintf(out + used, n - used, "%d,%ld,%ld,%ld;", i,
                   (long)g->d[b + 1], (long)g->d[b + 2], (long)g->d[b + 3]);
      if (wrote < 0 || (size_t)wrote >= n - used)
        break;
      used += wrote;
    }
  }
  if (!used && n)
    out[0] = 0;
}
