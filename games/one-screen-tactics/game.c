#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "One-Screen Tactics";
const char *game_rules =
    "Keep the beacon alive\nfor five turns.\nUP/DOWN choose a move.\nSELECT "
    "moves or shoves.\nPush enemies into X pits.\nTap a neighbor to act.";
static const int dx[] = {0, 1, 0, -1, 0}, dy[] = {-1, 0, 1, 0, 0};
static void transform(int *x, int *y, int b) {
  if (b & 4)
    *x = 4 - *x;
  for (int i = 0; i < (b & 3); i++) {
    int old = *x;
    *x = 4 - *y;
    *y = old;
  }
}
static bool hazard(const Game *g, int x, int y) {
  for (int i = 0; i < 2; i++)
    if (g->d[HAZARDS + i * 2] == x && g->d[HAZARDS + i * 2 + 1] == y)
      return true;
  return false;
}
static int enemy(const Game *g, int x, int y) {
  for (int i = 0; i < 3; i++)
    if (g->d[ENEMIES + i * 3 + 2] && g->d[ENEMIES + i * 3] == x &&
        g->d[ENEMIES + i * 3 + 1] == y)
      return i;
  return -1;
}
static void intents(Game *g) {
  for (int i = 0; i < 3; i++) {
    int base = INTENTS + i * 3;
    g->d[base + 2] = 0;
    if (!g->d[ENEMIES + i * 3 + 2])
      continue;
    int x = g->d[ENEMIES + i * 3], y = g->d[ENEMIES + i * 3 + 1];
    if (i == 0) {
      if (abs(x - g->d[BEACON_X]) + abs(y - g->d[BEACON_Y]) == 1) {
        g->d[base] = g->d[BEACON_X];
        g->d[base + 1] = g->d[BEACON_Y];
        g->d[base + 2] = 1;
      }
    } else if (i == 1 && g->d[ROUND] % 2 == 1) {
      g->d[base] = g->d[BEACON_X];
      g->d[base + 1] = g->d[BEACON_Y];
      g->d[base + 2] = 1;
    } else if (i == 2 && g->d[ROUND] % 3 == 2) {
      g->d[base] = g->d[PX];
      g->d[base + 1] = g->d[PY];
      g->d[base + 2] = 1;
    }
  }
}
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[BOARD] = seed % 8;
  g->stage = seed % 8;
  int coords[][2] = {{2, 3}, {1, 2}, {1, 3}, {4, 2}, {0, 0}, {0, 3}, {1, 4}};
  for (int i = 0; i < 7; i++)
    transform(&coords[i][0], &coords[i][1], g->d[BOARD]);
  g->d[PX] = coords[0][0];
  g->d[PY] = coords[0][1];
  g->d[BEACON_X] = coords[1][0];
  g->d[BEACON_Y] = coords[1][1];
  g->d[HEALTH] = 3;
  g->d[INTEGRITY] = 3;
  for (int i = 0; i < 3; i++) {
    g->d[ENEMIES + i * 3] = coords[i + 2][0];
    g->d[ENEMIES + i * 3 + 1] = coords[i + 2][1];
    g->d[ENEMIES + i * 3 + 2] = 1;
  }
  for (int i = 0; i < 2; i++) {
    g->d[HAZARDS + i * 2] = coords[i + 5][0];
    g->d[HAZARDS + i * 2 + 1] = coords[i + 5][1];
  }
  intents(g);
}
void tactics_act(Game *g, int dir) {
  if (g->status || dir < 0 || dir > 4)
    return;
  int x = g->d[PX] + dx[dir], y = g->d[PY] + dy[dir];
  if (x < 0 || x >= 5 || y < 0 || y >= 5)
    return;
  int foe = enemy(g, x, y);
  if (foe >= 0) {
    int xx = x + dx[dir], yy = y + dy[dir];
    if (xx < 0 || xx >= 5 || yy < 0 || yy >= 5 || enemy(g, xx, yy) >= 0)
      return;
    g->d[ENEMIES + foe * 3] = xx;
    g->d[ENEMIES + foe * 3 + 1] = yy;
    if (hazard(g, xx, yy)) {
      g->d[ENEMIES + foe * 3 + 2] = 0;
      g->score += 100;
    }
  } else {
    g->d[PX] = x;
    g->d[PY] = y;
    if (hazard(g, x, y)) {
      g->d[HEALTH] = 0;
      g->status = 2;
      return;
    }
  }
  for (int i = 0; i < 3; i++) {
    int b = INTENTS + i * 3;
    if (!g->d[ENEMIES + i * 3 + 2] || !g->d[b + 2])
      continue;
    if (g->d[b] == g->d[PX] && g->d[b + 1] == g->d[PY])
      g->d[HEALTH]--;
    if (g->d[b] == g->d[BEACON_X] && g->d[b + 1] == g->d[BEACON_Y])
      g->d[INTEGRITY]--;
  }
  g->d[ROUND]++;
  g->ticks++;
  if (g->d[HEALTH] <= 0 || g->d[INTEGRITY] <= 0)
    g->status = 2;
  else if (g->d[ROUND] == 5) {
    g->status = 1;
    g->score += g->d[HEALTH] * 50 + g->d[INTEGRITY] * 100;
  } else
    intents(g);
}
void game_input(Game *g, int a) {
  if (g->status)
    return;
  if (a == ACT_UP)
    g->d[CHOICE] = (g->d[CHOICE] + 4) % 5;
  else if (a == ACT_DOWN)
    g->d[CHOICE] = (g->d[CHOICE] + 1) % 5;
  else if (a == ACT_SELECT)
    tactics_act(g, g->d[CHOICE]);
}
void game_touch(Game *g, int x, int y, int type) {
  if (type != 2)
    return;
  x = (x - 18) / 28;
  y = (y - 2) / 28;
  if (x < 0 || x >= 5 || y < 0 || y >= 5)
    return;
  for (int i = 0; i < 5; i++)
    if (g->d[PX] + dx[i] == x && g->d[PY] + dy[i] == y) {
      g->d[CHOICE] = i;
      tactics_act(g, i);
      break;
    }
}
void game_tick(Game *g) { (void)g; }
bool game_running(const Game *g) {
  (void)g;
  return false;
}
bool game_valid(const Game *g) {
  return g->status >= 0 && g->status <= 2 && g->d[PX] >= 0 && g->d[PX] < 5 &&
         g->d[PY] >= 0 && g->d[PY] < 5 && g->d[ROUND] >= 0 &&
         g->d[ROUND] <= 5 && g->d[CHOICE] >= 0 && g->d[CHOICE] < 5 &&
         g->d[HEALTH] >= -3 && g->d[HEALTH] <= 3 && g->d[INTEGRITY] >= -3 &&
         g->d[INTEGRITY] <= 3;
}
void game_hud(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, "HP %ld  Beacon %ld  %ld/5", (long)g->d[HEALTH],
           (long)g->d[INTEGRITY], (long)g->d[ROUND]);
  const char *names[] = {"UP", "RIGHT", "DOWN", "LEFT", "WAIT"};
  snprintf(b, m, "%s   SELECT: act", names[g->d[CHOICE]]);
}

const int game_controls = 0;

const int game_continuous = 0;
