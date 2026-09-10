#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "Pocket Ecosystem";
const char *game_rules =
    "Add a hunter.\nKeep 3 grazers + plants\nbalanced for 60 seconds.\nUP/DOWN "
    "cursor; SEL place.\nHold SEL changes tool.\nTap a tool, then a cell.";
const int game_controls = 0;
static int occupied(const Game *g, int cell) {
  for (int i = 0; i < SLOTS; i++)
    if (g->d[ANIMALS + i * 5] && g->d[ANIMALS + i * 5 + 1] == cell)
      return i;
  return -1;
}
static bool add(Game *g, int kind, int cell) {
  if (occupied(g, cell) >= 0)
    return false;
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5;
    if (!g->d[b]) {
      g->d[b] = kind;
      g->d[b + 1] = cell;
      g->d[b + 2] = kind == 1 ? 10 : 22;
      g->d[b + 3] = 8;
      return true;
    }
  }
  return false;
}
void ecosystem_counts(const Game *g, int *plants, int *herb, int *carn) {
  *plants = *herb = *carn = 0;
  for (int i = 0; i < 24; i++)
    *plants += g->d[PLANTS + i];
  for (int i = 0; i < SLOTS; i++) {
    *herb += g->d[ANIMALS + i * 5] == 1;
    *carn += g->d[ANIMALS + i * 5] == 2;
  }
}
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[WATER] = 70;
  g->d[BUDGET] = 10;
  for (int i = 0; i < 24; i++)
    g->d[PLANTS + i] = 3 + rnd(g) % 4;
  add(g, 1, 1);
  add(g, 1, 8);
}
void game_input(Game *g, int a) {
  if (g->status == 2)
    return;
  if (a == ACT_UP)
    g->d[CURSOR] = (g->d[CURSOR] + 23) % 24;
  else if (a == ACT_DOWN)
    g->d[CURSOR] = (g->d[CURSOR] + 1) % 24;
  else if (a == ACT_SPECIAL)
    g->d[TOOL] = (g->d[TOOL] + 1) % 4;
  else if (a == ACT_SELECT) {
    int tool = g->d[TOOL], cost = tool == 0   ? 2
                                  : tool == 1 ? 3
                                  : tool == 2 ? 5
                                              : 3;
    if (g->d[BUDGET] < cost)
      return;
    bool placed = true;
    if (tool == 0)
      g->d[PLANTS + g->d[CURSOR]] = 6;
    else if (tool == 3)
      g->d[WATER] = limit(g->d[WATER] + 35, 0, 100);
    else
      placed = add(g, tool, g->d[CURSOR]);
    if (placed)
      g->d[BUDGET] -= cost;
  }
}
void game_touch(Game *g, int x, int y, int type) {
  if (type != 2)
    return;
  if (y >= 114) {
    g->d[TOOL] = limit(x / 44, 0, 3);
    if (g->d[TOOL] == 3)
      game_input(g, ACT_SELECT);
    return;
  }
  if (x < 4 || x >= 172 || y < 1 || y >= 113)
    return;
  g->d[CURSOR] = (y - 1) / 28 * 6 + (x - 4) / 28;
  game_input(g, ACT_SELECT);
}
static int neighbor(int cell, int dir) {
  int x = cell % 6, y = cell / 6;
  const int dx[] = {0, 1, 0, -1}, dy[] = {-1, 0, 1, 0};
  x += dx[dir];
  y += dy[dir];
  return x < 0 || x >= 6 || y < 0 || y >= 4 ? -1 : y * 6 + x;
}
void game_tick(Game *g) {
  if (g->status == 2)
    return;
  g->ticks++;
  if (g->ticks % 30)
    return;
  int sec = ++g->d[SECONDS];
  g->stage = (sec / 30) % 4;
  if (sec % 8 == 0)
    g->d[BUDGET] = limit(g->d[BUDGET] + 1, 0, 10);
  g->d[WATER] = limit(g->d[WATER] + (g->stage == 0   ? 2
                                     : g->stage == 1 ? -2
                                     : g->stage == 2 ? 1
                                                     : 0),
                      0, 100);
  for (int k = 0; k < (g->stage == 3 ? 4 : 12); k++) {
    int cell = rnd(g) % 24;
    if (g->d[PLANTS + cell] < 6 && g->d[WATER] > 10 &&
        (g->d[PLANTS + cell] || rnd(g) % 8 == 0))
      g->d[PLANTS + cell]++;
  }
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5, kind = g->d[b];
    if (!kind)
      continue;
    if (g->d[b + 3])
      g->d[b + 3]--;
    if (kind == 2 && sec % 4)
      continue;
    g->d[b + 2]--;
    int cell = g->d[b + 1], best = cell, best_score = -1000;
    for (int d = 0; d < 5; d++) {
      int n = d == 4 ? cell : neighbor(cell, d);
      if (n < 0)
        continue;
      int who = occupied(g, n);
      if (who >= 0 && who != i && !(kind == 2 && g->d[ANIMALS + who * 5] == 1))
        continue;
      int score = 0;
      if (kind == 1)
        score = g->d[PLANTS + n] * 5 + (int)(rnd(g) % 3);
      else {
        score = -100;
        for (int j = 0; j < SLOTS; j++)
          if (g->d[ANIMALS + j * 5] == 1) {
            int prey = g->d[ANIMALS + j * 5 + 1],
                distance = abs(n % 6 - prey % 6) + abs(n / 6 - prey / 6);
            if (-distance > score)
              score = -distance;
          }
      }
      if (score > best_score) {
        best_score = score;
        best = n;
      }
    }
    if (kind == 2) {
      int prey = occupied(g, best);
      if (prey >= 0 && prey != i && g->d[ANIMALS + prey * 5] == 1) {
        g->d[ANIMALS + prey * 5] = 0;
        g->d[b + 2] += 9;
      }
    }
    g->d[b + 1] = best;
    if (kind == 1 && g->d[PLANTS + best] > 0) {
      g->d[PLANTS + best]--;
      g->d[b + 2] += 3;
    }
    g->d[b + 2] = limit(g->d[b + 2], 0, 40);
    if (!g->d[b + 2]) {
      g->d[b] = 0;
      continue;
    }
    if (!g->d[b + 3] && g->d[b + 2] > (kind == 1 ? 15 : 30)) {
      int pl, h, p;
      ecosystem_counts(g, &pl, &h, &p);
      if ((kind == 1 && h < 9) || (kind == 2 && p < 2)) {
        for (int d = 0; d < 4; d++) {
          int n = neighbor(best, d);
          if (n >= 0 && occupied(g, n) < 0 && add(g, kind, n)) {
            g->d[b + 2] /= 2;
            g->d[b + 3] = kind == 1 ? 10 : 18;
            break;
          }
        }
      }
    }
  }
  int pl, h, p;
  ecosystem_counts(g, &pl, &h, &p);
  if (pl >= 20 && h >= 3 && p >= 1 && g->d[WATER] > 10) {
    g->d[BALANCE]++;
    if (g->d[BALANCE] >= 60) {
      g->status = 1;
      g->score = g->d[BALANCE];
    }
  } else
    g->d[BALANCE] = 0;
  if (!h || !p)
    g->d[EXTINCT]++;
  else
    g->d[EXTINCT] = 0;
  if (g->d[EXTINCT] >= 40)
    g->status = 2;
}
bool game_running(const Game *g) { return g->status != 2; }
bool game_valid(const Game *g) {
  if (g->status < 0 || g->status > 2 || g->d[CURSOR] < 0 ||
      g->d[CURSOR] >= 24 || g->d[TOOL] < 0 || g->d[TOOL] > 3 ||
      g->d[WATER] < 0 || g->d[WATER] > 100 || g->d[BUDGET] < 0 ||
      g->d[BUDGET] > 10)
    return false;
  for (int i = 0; i < 24; i++)
    if (g->d[PLANTS + i] < 0 || g->d[PLANTS + i] > 6)
      return false;
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5;
    if (g->d[b] < 0 || g->d[b] > 2 || g->d[b + 1] < 0 || g->d[b + 1] >= 24 ||
        g->d[b + 2] < 0 || g->d[b + 2] > 40)
      return false;
  }
  return true;
}
void game_hud(const Game *g, char *a, size_t n, char *b, size_t m) {
  int pl, h, p;
  ecosystem_counts(g, &pl, &h, &p);
  const char *seasons[] = {"Spring", "Summer", "Autumn", "Winter"};
  const char *tools[] = {"Leaf 2", "Graze 3", "Hunt 5", "Rain 3"};
  snprintf(a, n, "%s L%d G%d P%d", seasons[g->stage], pl, h, p);
  if (g->status == 2)
    snprintf(b, m, !h ? "Grazers extinct" : "Hunters extinct");
  else if (!p)
    snprintf(b, m, "Add a hunter! Budget $%ld", (long)g->d[BUDGET]);
  else if (h < 3)
    snprintf(b, m, "Need 3 grazers. Budget $%ld", (long)g->d[BUDGET]);
  else if (g->status == 1)
    snprintf(b, m, "Balanced! Water %ld  $%ld", (long)g->d[WATER],
             (long)g->d[BUDGET]);
  else
    snprintf(b, m, "%s $%ld  W%ld  %ld/60", tools[g->d[TOOL]],
             (long)g->d[BUDGET], (long)g->d[WATER], (long)g->d[BALANCE]);
}

const int game_continuous = 1;

void game_debug(const Game *g, char *out, size_t n) {
  (void)g;
  if (n)
    out[0] = 0;
}

void game_result(const Game *g, char *title, size_t n, char *detail, size_t m) {
  int plants, grazers, hunters;
  ecosystem_counts(g, &plants, &grazers, &hunters);
  snprintf(title, n, "%s", !grazers ? "Grazers extinct" : "Hunters extinct");
  snprintf(detail, m, "Habitat lasted %ldm %lds", (long)g->d[SECONDS] / 60,
           (long)g->d[SECONDS] % 60);
}
