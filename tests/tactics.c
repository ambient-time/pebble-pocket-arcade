#include "model.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static int solve(Game g, int depth) {
  if (g.status)
    return g.status == 1;
  if (!depth)
    return 0;
  for (int a = 0; a < 5; a++) {
    Game h = g;
    tactics_act(&h, a);
    assert(game_valid(&h));
    if (!memcmp(&h, &g, sizeof g))
      continue;
    if (solve(h, depth - 1))
      return 1;
  }
  return 0;
}
int main(void) {
  for (int b = 0; b < 8; b++) {
    Game g;
    game_init(&g, b);
    assert(solve(g, 5));
    for (int i = 0; i < 5; i++)
      tactics_act(&g, 4);
    assert(g.status == 2);
  }
  Game g;
  game_init(&g, 0);
  tactics_act(&g, 3);
  assert(!g.d[ENEMIES + 2]);
  assert(g.score == 100);
  assert(g.d[INTEGRITY] == 3);
  puts("PASS: all 8 boards solvable within five turns; all 8 idle lines lose; "
       "hazard shove removes attacker before its telegraphed attack");
}
