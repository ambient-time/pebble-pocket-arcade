#include "model.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
  int balanced = 0;
  for (int seed = 0; seed < 100; seed++) {
    Game g;
    game_init(&g, seed);
    g.d[TOOL] = 1;
    g.d[CURSOR] = 15;
    game_input(&g, ACT_SELECT);
    g.d[TOOL] = 2;
    g.d[CURSOR] = 5;
    game_input(&g, ACT_SELECT);
    for (int t = 0; t < 18000 && g.status != 2; t++) {
      if (g.d[WATER] < 30 && g.d[BUDGET] >= 3) {
        g.d[TOOL] = 3;
        game_input(&g, ACT_SELECT);
      }
      game_tick(&g);
      assert(game_valid(&g));
      if (g.status == 1) {
        balanced++;
        break;
      }
    }
  }
  printf("Ecosystem species introduction and rain policy: %d/100 balanced\n",
         balanced);
  assert(balanced > 20);
  Game g;
  game_init(&g, 1);
  for (int t = 0; t < 1200; t++)
    game_tick(&g);
  assert(g.status == 2);
  game_init(&g, 1);
  for (int i = 0; i < 24; i++)
    g.d[PLANTS + i] = 0;
  for (int t = 0; t < 6000 && g.status != 2; t++)
    game_tick(&g);
  assert(g.status == 2);
  game_init(&g, 1);
  g.d[BUDGET] = 0;
  g.d[TOOL] = 3;
  int water = g.d[WATER];
  game_input(&g, ACT_SELECT);
  assert(g.d[WATER] == water);
  puts("PASS: carrying bounds, starvation/collapse, intervention costs and "
       "achievable balance");
}
