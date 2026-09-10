#include "model.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  Game g;
  game_init(&g, argc > 1 ? strtoul(argv[1], 0, 10) : 1);
  for (int turn = 0; !g.status && turn < 30; turn++) {
    int a, p;
    artillery_solution(&g, 0, &a, &p);
    artillery_launch(&g, 0, a, p);
    for (int t = 0; game_running(&g) && t < 2000; t++)
      game_tick(&g);
    printf("%d %d %d %d %d\n", a, p, g.d[HP], g.d[ENEMY_HP], g.status);
  }
  return g.status == 1 ? 0 : 1;
}
