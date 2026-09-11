// Native readability contract. Luke Steuber.
#pragma once
#include <pebble.h>
#include "model.h"
#define UI_APP_VERSION "0.3.1"
#define UI_FONT_SMALL FONT_KEY_GOTHIC_18_BOLD
#define UI_FONT_BODY FONT_KEY_GOTHIC_24_BOLD
#define UI_FONT_TITLE FONT_KEY_GOTHIC_28_BOLD
#define UI_ROW_HEIGHT 38
// World coordinates remain unchanged in saves and physics.
#define UI_FIELD_SCALE PBL_IF_ROUND_ELSE(UI_ROUND_SCALE, 108)
