// Native readability contract. Luke Steuber.
#pragma once
#include <pebble.h>
#include "model.h"
#define UI_APP_VERSION "0.4.0"
#define UI_FONT_SMALL FONT_KEY_GOTHIC_18_BOLD
#define UI_FONT_BODY FONT_KEY_GOTHIC_24_BOLD
#define UI_FONT_TITLE FONT_KEY_GOTHIC_28_BOLD
#define UI_ROW_HEIGHT 38
// Only Time 2 and Round 2 have touchscreens. The other four watches are
// 144x168 rectangles or the 180x180 Time Round; they get compact copy and an
// 18 px body size so rules and results fit without truncation.
#if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_GABBRO)
#define UI_TOUCH 1
#define UI_COMPACT 0
#else
#define UI_TOUCH 0
#define UI_COMPACT 1
#endif
#define UI_FONT_PROSE (UI_COMPACT ? UI_FONT_SMALL : UI_FONT_BODY)
// Field scale per screen. World coordinates remain unchanged in saves and
// physics; only the projection changes. 81% puts all 176 world units across a
// 144-pixel screen. Games may override the round sizes and placement.
#ifndef UI_SMALL_SCALE
#define UI_SMALL_SCALE 81
#endif
#ifndef UI_CHALK_SCALE
#define UI_CHALK_SCALE (UI_ROUND_SCALE * 180 / 260)
#endif
#if defined(PBL_PLATFORM_EMERY)
#define UI_FIELD_SCALE 108
#elif defined(PBL_PLATFORM_GABBRO)
#define UI_FIELD_SCALE UI_ROUND_SCALE
#elif defined(PBL_PLATFORM_CHALK)
#define UI_FIELD_SCALE UI_CHALK_SCALE
#else
#define UI_FIELD_SCALE UI_SMALL_SCALE
#endif
// Top of the first text row. Round screens start lower, inside the rim.
#if defined(PBL_PLATFORM_EMERY)
#define UI_TOP 6
#elif defined(PBL_PLATFORM_GABBRO)
#define UI_TOP 22
#elif defined(PBL_PLATFORM_CHALK)
#define UI_TOP 10
#else
#define UI_TOP 2
#endif
// In-game top caption: a little lower on the Time Round, where the chord at
// the very top is too short for a score line.
#if defined(PBL_PLATFORM_CHALK)
#define UI_HUD_TOP (UI_TOP + 5)
#else
#define UI_HUD_TOP UI_TOP
#endif
// Vertical field origin on round screens; default centres the field.
#ifndef UI_ROUND_FIELD_Y
#define UI_ROUND_FIELD_Y(h) (((h) - FIELD_H * UI_FIELD_SCALE / 100) / 2)
#endif
// Optional shorter rules and HUD copy for compact screens.
#ifndef GAME_RULES_COMPACT
#define GAME_RULES_COMPACT game_rules
#endif
// Optional title for the compact How to play screen (for example, with a line
// break where the name is wider than the Time Round's chord).
#ifndef GAME_NAME_COMPACT
#define GAME_NAME_COMPACT game_name
#endif
