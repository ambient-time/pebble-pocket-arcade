#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int path[5];
static int solve(Game g, int depth) {
  if (g.status)
    return g.status == 1;
  if (depth == 5)
    return 0;
  for (int a = 0; a < 5; a++) {
    Game h = g;
    tactics_act(&h, a);
    if (!memcmp(&h, &g, sizeof g))
      continue;
    path[depth] = a;
    if (solve(h, depth + 1))
      return 1;
  }
  return 0;
}
int main(int argc, char **argv) {
  Game g;
  game_init(&g, argc > 1 ? atoi(argv[1]) : 0);
  if (!solve(g, 0))
    return 1;
  for (int i = 0; i < 5; i++)
    printf("%d ", path[i]);
  puts("");
}
