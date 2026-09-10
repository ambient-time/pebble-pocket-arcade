// Two banks; commit selector after data and checksum. Luke Steuber.
#include "save.h"
#include <string.h>
static Game scratch;
static uint32_t hash(const void *v, size_t n) {
  const uint8_t *p = v;
  uint32_t h = 2166136261u;
  while (n--)
    h = (h ^ *p++) * 16777619u;
  return h;
}
static int selected(const SaveIO *io) {
  int32_t b = 0;
  return io->read(50, &b, sizeof b) == sizeof b && b == 1 ? 1 : 0;
}
static bool bank_load(const SaveIO *io, int bank, Game *g) {
  uint32_t base = 100 + bank * 32, meta[3];
  if (io->read(base + 31, meta, sizeof meta) != sizeof meta ||
      meta[0] != 0x50474131 || meta[1] != sizeof *g)
    return false;
  for (unsigned o = 0; o < sizeof *g; o += 240) {
    unsigned n = sizeof *g - o;
    if (n > 240)
      n = 240;
    if (io->read(base + o / 240, (uint8_t *)g + o, n) != (int)n)
      return false;
  }
  return hash(g, sizeof *g) == meta[2] && game_valid(g);
}
bool save_load(const SaveIO *io, Game *g) {
  int b = selected(io);
  return bank_load(io, b, g) || bank_load(io, b ^ 1, g);
}
bool save_store(const SaveIO *io, const Game *g) {
  if (!game_valid(g))
    return false;
  int32_t bank = selected(io) ^ 1;
  if (!bank_load(io, bank ^ 1, &scratch) && bank_load(io, bank, &scratch)) {
    if (io->write(50, &bank, sizeof bank) != sizeof bank)
      return false;
    bank ^= 1;
  }
  uint32_t base = 100 + bank * 32;
  for (unsigned o = 0; o < sizeof *g; o += 240) {
    unsigned n = sizeof *g - o;
    if (n > 240)
      n = 240;
    if (io->write(base + o / 240, (const uint8_t *)g + o, n) != (int)n)
      return false;
  }
  uint32_t meta[] = {0x50474131, sizeof *g, hash(g, sizeof *g)};
  if (io->write(base + 31, meta, sizeof meta) != sizeof meta)
    return false;
  return io->write(50, &bank, sizeof bank) == sizeof bank;
}
