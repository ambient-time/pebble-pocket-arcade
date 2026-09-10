#pragma once
#include "arcade.h"
typedef struct {
  int (*read)(uint32_t, void *, size_t);
  int (*write)(uint32_t, const void *, size_t);
} SaveIO;
bool save_load(const SaveIO *, Game *);
bool save_store(const SaveIO *, const Game *);
