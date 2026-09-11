// Native watch shell shared by six games. Luke Steuber.
#include "arcade.h"
#include "draw.h"
#include "save.h"
static Game g;
static Window *win;
static Layer *canvas;
static MenuLayer *menu;
static AppTimer *timer, *replay_timer, *exit_timer;
static bool result_visible, replay_ready, replay_touch, replay_select;
static int replay_x, replay_y;
static bool touch_on, focused = true, save_error, touching, exiting;
static int screen = 2, view_w, view_h;
static bool menu_visible, config_set;
static int config_kind = -1; // 0 play, 1 pause, 2 rules, 3 restart confirmation
int field_x, field_y;
static const SaveIO io = {persist_read_data, persist_write_data};
static const char *rows[] = {"Resume game", "Restart game", "How to play",
                             "Save and exit"};
void ink(GContext *c, GColor color) {
  graphics_context_set_fill_color(c, color);
  graphics_context_set_stroke_color(c, color);
  graphics_context_set_text_color(c, color);
}
static int scale(int v) { return v * UI_FIELD_SCALE / 100; }
static int world_x(int x) { return (x - field_x) * 100 / UI_FIELD_SCALE; }
static int world_y(int y) { return (y - field_y) * 100 / UI_FIELD_SCALE; }
void line(GContext *c, int x, int y, int xx, int yy) {
  graphics_draw_line(c, GPoint(field_x + scale(x), field_y + scale(y)),
                     GPoint(field_x + scale(xx), field_y + scale(yy)));
}
void box(GContext *c, int x, int y, int w, int h, bool fill) {
  GRect r = GRect(field_x + scale(x), field_y + scale(y),
                  scale(x + w) - scale(x), scale(y + h) - scale(y));
  if (fill)
    graphics_fill_rect(c, r, 0, GCornerNone);
  else
    graphics_draw_rect(c, r);
}
void circle(GContext *c, int x, int y, int r, bool fill) {
  if (fill)
    graphics_fill_circle(c, GPoint(field_x + scale(x), field_y + scale(y)), scale(r));
  else
    graphics_draw_circle(c, GPoint(field_x + scale(x), field_y + scale(y)), scale(r));
}
static void field_text(GContext *c, const char *s, int x, int y, int width, bool large) {
  graphics_draw_text(c, s, fonts_get_system_font(large ? UI_FONT_BODY : UI_FONT_SMALL),
                     GRect(field_x + scale(x), field_y + scale(y), scale(width), 32),
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter,
                     NULL);
}
void label(GContext *c, const char *s, int x, int y, int width) {
  field_text(c, s, x, y, width, false);
}
void large_label(GContext *c, const char *s, int x, int y, int width) {
  field_text(c, s, x, y, width, true);
}
static void text(GContext *c, const char *s, int y, int height, bool large) {
  ink(c, GColorWhite);
  graphics_draw_text(c, s,
                     fonts_get_system_font(large ? UI_FONT_TITLE : UI_FONT_BODY),
                     GRect(10, y, view_w - 20, height),
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}
static void caption(GContext *c, const char *s, int y) {
  ink(c, GColorWhite);
  int width = PBL_IF_ROUND_ELSE(186, 190);
  graphics_draw_text(c, s, fonts_get_system_font(UI_FONT_SMALL),
                     GRect((view_w - width) / 2, y, width, 24),
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
static bool save(void) {
  save_error = !save_store(&io, &g);
  return !save_error;
}
static void log_state(void) {
  APP_LOG(
      APP_LOG_LEVEL_INFO,
      "AR s=%d st=%d stage=%d t=%lu sc=%lu a=%ld b=%ld c=%ld d=%ld e=%ld f=%ld",
      screen, g.status, g.stage, (unsigned long)g.ticks, (unsigned long)g.score,
      (long)g.d[0], (long)g.d[1], (long)g.d[2], (long)g.d[3], (long)g.d[5],
      (long)g.d[6]);
  char detail[220];
  game_debug(&g, detail, sizeof detail);
  for (unsigned i = 0; detail[i];) {
    char chunk[61];
    unsigned n = 0;
    while (detail[i] && n < 60)
      chunk[n++] = detail[i++];
    chunk[n] = 0;
    APP_LOG(APP_LOG_LEVEL_INFO, "AR part %s", chunk);
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "AR end");
}
static void schedule(void);
static void sync(void) {
  bool show = screen == 1 && !save_error;
  if (show != menu_visible) {
    menu_visible = show;
    touching = false;
    layer_set_hidden(menu_layer_get_layer(menu), !show);
    if (touch_on) {
      app_touch_navigation_enable(show);
      window_set_touch_bridge_disabled(win, !show);
    }
    if (show)
      menu_layer_set_selected_index(menu, (MenuIndex){0, 0}, MenuRowAlignTop,
                                    false);
  }
}
static void clicks(void *);
static bool ended(void) {
  return g.status && !(game_continuous && g.status == 1);
}
static bool replay_hit(int x, int y) {
  x = world_x(x); y = world_y(y);
  return x >= 22 && x < 154 && y >= 100 && y < 134;
}
static void ready_to_replay(void *context) {
  replay_timer = NULL;
  replay_ready = result_visible;
  if (canvas && !exiting) layer_mark_dirty(canvas);
}
static void sync_result(void) {
  bool finished = ended();
  if (finished == result_visible)
    return;
  result_visible = finished;
  touching = replay_touch = replay_ready = replay_select = false;
  if (replay_timer) {
    app_timer_cancel(replay_timer);
    replay_timer = NULL;
  }
  if (finished) {
    game_input(&g, ACT_RELEASE_UP);
    game_input(&g, ACT_RELEASE_DOWN);
    game_input(&g, ACT_RELEASE_SELECT);
    replay_timer = app_timer_register(550, ready_to_replay, NULL);
  }
}
static void refresh(void) {
  if (!canvas || exiting) return;
  sync_result();
  sync();
  int kind = !screen && ended() ? 2 : game_controls && !screen && !g.status;
  if (!config_set || kind != config_kind) {
    window_set_click_config_provider(win, clicks);
    config_set = true;
    config_kind = kind;
  }
  layer_mark_dirty(canvas);
  log_state();
  schedule();
}
static void draw(Layer *l, GContext *c) {
  ink(c, GColorBlack);
  graphics_fill_rect(c, layer_get_bounds(l), 0, GCornerNone);
  int top = PBL_IF_ROUND_ELSE(22, 6);
  graphics_context_set_antialiased(c, false);
  if (save_error) {
    text(c, "Save paused", top + 18, 35, true);
    text(c, "Progress is still here.\nSELECT retries saving.", top + 60, 100,
         false);
    return;
  }
  if (screen == 2) {
    text(c, game_name, top, 32, false);
    text(c, game_rules, top + 36, 146, false);
    caption(c, "SELECT / tap: play", view_h - PBL_IF_ROUND_ELSE(39, 27));
    return;
  }
  if (screen == 3) {
    text(c, "Restart game?", top + 28, 36, true);
    text(c, "SELECT: start fresh\nBACK: keep game", top + 90, 65, false);
    return;
  }
  if (screen == 1) return; // The native list owns the complete pause surface.
  char a[64], b[64];
  game_hud(&g, a, sizeof a, b, sizeof b);
  caption(c, a, top);
  game_draw(&g, c);
  caption(c, ended() ? "BACK: menu" : b, field_y + scale(FIELD_H) + 1);
  if (ended()) {
    char title[32], detail[64];
    game_result(&g, title, sizeof title, detail, sizeof detail);
    ink(c, GColorBlack);
    box(c, 6, 20, 164, 116, true);
    ink(c, g.status == 1 ? GColorCyan : GColorWhite);
    box(c, 6, 20, 164, 116, false);
    graphics_draw_text(c, title, fonts_get_system_font(UI_FONT_BODY),
                       GRect(field_x + scale(5), field_y + scale(22), scale(166), 32),
                       GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    ink(c, GColorWhite);
    graphics_draw_text(c, detail, fonts_get_system_font(UI_FONT_BODY),
                       GRect(field_x + scale(8), field_y + scale(52), scale(160), 55),
                       GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    ink(c, replay_ready ? GColorWhite : GColorLightGray);
    box(c, 22, 100, 132, 34, true);
    ink(c, GColorBlack);
    large_label(c, "Play again", 24, 102, 128);
  }
}
static void tick(void *x) {
  timer = NULL;
  if (!focused || !canvas || exiting || screen || save_error)
    return;
  bool before = game_running(&g);
  int status = g.status;
  game_tick(&g);
  if ((before && !game_running(&g)) || status != g.status || g.ticks % 300 == 0)
    save();
  if (g.ticks % (game_controls ? 3 : 30) == 0 || status != g.status ||
      (before && !game_running(&g)))
    log_state();
  if (status != g.status)
    refresh();
  else {
    layer_mark_dirty(canvas);
    schedule();
  }
}
static void schedule(void) {
  bool run = focused && canvas && !exiting && !screen && !save_error && game_running(&g);
  if (!run && timer) {
    app_timer_cancel(timer);
    timer = NULL;
  }
  if (run && !timer)
    timer = app_timer_register(33, tick, NULL);
}
static void menu_select(MenuLayer *, MenuIndex *, void *);
static void new_game(void) {
  uint32_t seed = (uint32_t)time(NULL);
  APP_LOG(APP_LOG_LEVEL_INFO, "AR newseed=%lu", (unsigned long)seed);
  game_init(&g, seed);
}
static void action(int a) {
  if (!focused || !canvas || exiting)
    return;
  if (save_error) {
    if (a == ACT_SELECT) {
      save();
      refresh();
    }
    return;
  }
  if (screen == 1) {
    MenuIndex i = menu_layer_get_selected_index(menu);
    if (a == ACT_UP)
      i.row = (i.row + 3) % 4;
    else if (a == ACT_DOWN)
      i.row = (i.row + 1) % 4;
    else if (a == ACT_SELECT) {
      menu_select(menu, &i, NULL);
      return;
    }
    menu_layer_set_selected_index(menu, i, MenuRowAlignCenter, false);
    refresh();
    return;
  }
  if (screen == 2) {
    screen = 0;
    persist_write_int(51, 1);
  } else if (screen == 3) {
    if (a == ACT_SELECT) {
      new_game();
      screen = 0;
    }
  } else if (g.status && !(game_continuous && g.status == 1)) {
    if (a == ACT_SELECT && replay_ready)
      new_game();
  } else
    game_input(&g, a);
  if (!game_controls || screen || !game_running(&g))
    save();
  refresh();
}
static void up(ClickRecognizerRef r, void *x) { action(ACT_UP); }
static void down(ClickRecognizerRef r, void *x) { action(ACT_DOWN); }
static void select_click(ClickRecognizerRef r, void *x) { action(ACT_SELECT); }
static void special(ClickRecognizerRef r, void *x) { action(ACT_SPECIAL); }
static void back(ClickRecognizerRef r, void *x) {
  if (!focused || !canvas || exiting || save_error)
    return;
  if (screen == 3 || screen == 2 || screen == 1)
    screen = 0;
  else
    screen = 1;
  game_input(&g, ACT_RELEASE_UP);
  game_input(&g, ACT_RELEASE_DOWN);
  touching = false;
  save();
  refresh();
}
static void left_press(ClickRecognizerRef r, void *x) { action(ACT_UP); }
static void left_release(ClickRecognizerRef r, void *x) {
  game_input(&g, ACT_RELEASE_UP);
}
static void right_press(ClickRecognizerRef r, void *x) { action(ACT_DOWN); }
static void right_release(ClickRecognizerRef r, void *x) {
  game_input(&g, ACT_RELEASE_DOWN);
}
static void replay_press(ClickRecognizerRef r, void *x) {
  replay_select = replay_ready && ended() && !screen;
}
static void replay_release(ClickRecognizerRef r, void *x) {
  bool fresh_press = replay_select;
  replay_select = false;
  if (fresh_press)
    action(ACT_SELECT);
}
static void clicks(void *x) {
  if (!screen && ended()) {
    window_raw_click_subscribe(BUTTON_ID_SELECT, replay_press, replay_release,
                               NULL);
    window_single_click_subscribe(BUTTON_ID_BACK, back);
    return;
  }
  if (game_controls && !screen &&
      (!g.status || (game_continuous && g.status == 1))) {
    window_raw_click_subscribe(BUTTON_ID_UP, left_press, left_release, NULL);
    window_raw_click_subscribe(BUTTON_ID_DOWN, right_press, right_release,
                               NULL);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click);
    window_single_click_subscribe(BUTTON_ID_BACK, back);
    return;
  }
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, up);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click);
  window_long_click_subscribe(BUTTON_ID_SELECT, 450, special, NULL);
  window_single_click_subscribe(BUTTON_ID_BACK, back);
}
static uint16_t row_count(MenuLayer *m, uint16_t section, void *x) { return 4; }
static int16_t row_height(MenuLayer *m, MenuIndex *i, void *x) { return UI_ROW_HEIGHT; }
static void draw_row(GContext *c, const Layer *l, MenuIndex *i, void *x) {
  GRect r = layer_get_bounds(l);
  graphics_draw_text(c, rows[i->row], fonts_get_system_font(UI_FONT_BODY),
      GRect(4, 2, r.size.w - 8, UI_ROW_HEIGHT - 4),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
static void exit_after_callback(void *context) {
  exit_timer = NULL;
  window_stack_pop_all(false);
}
static void request_exit(void) {
  touching = replay_touch = replay_select = false;
  if (timer) { app_timer_cancel(timer); timer = NULL; }
  if (replay_timer) { app_timer_cancel(replay_timer); replay_timer = NULL; }
  // Firmware accesses MenuLayer after select_click returns.
  exit_timer = app_timer_register(1, exit_after_callback, NULL);
  exiting = exit_timer != NULL;
  if (!exiting) refresh();
}
static void menu_select(MenuLayer *m, MenuIndex *i, void *x) {
  if (!focused || !canvas || exiting) return;
  int row = i->row;
  if (row == 0)
    screen = 0;
  else if (row == 1)
    screen = 3;
  else if (row == 2)
    screen = 2;
  else if (save()) {
    request_exit();
    return;
  }
  refresh();
}
static void touch(const TouchEvent *e, void *x) {
  if (!focused || !canvas || exiting)
    return;
  if (save_error) {
    if (e->type == TouchEvent_Touchdown)
      touching = true;
    else if (e->type != TouchEvent_PositionUpdate && touching) {
      touching = false;
      action(ACT_SELECT);
    }
    return;
  }
  if (screen == 1)
    return;
  if (e->type == TouchEvent_Touchdown) {
    touching = true;
    replay_x = e->x;
    replay_y = e->y;
    replay_touch = !screen && ended() && replay_ready && replay_hit(e->x, e->y);
    if (!screen && (!g.status || (game_continuous && g.status == 1))) {
      game_touch(&g, world_x(e->x), world_y(e->y), 0);
      schedule();
      layer_mark_dirty(canvas);
    }
  } else if (e->type == TouchEvent_PositionUpdate) {
    if (abs(e->x - replay_x) > 8 || abs(e->y - replay_y) > 8)
      replay_touch = false;
    if (touching && !screen &&
        (!g.status || (game_continuous && g.status == 1)))
      game_touch(&g, world_x(e->x), world_y(e->y), 1);
  } else if (touching) {
    touching = false;
    if (screen == 2 || save_error)
      action(ACT_SELECT);
    else if (screen == 3) {
      if (e->y > view_h / 2)
        action(ACT_SELECT);
    } else if (g.status && !(game_continuous && g.status == 1)) {
      bool intentional = replay_touch && replay_hit(e->x, e->y) &&
                         abs(e->x - replay_x) <= 8 && abs(e->y - replay_y) <= 8;
      replay_touch = false;
      if (intentional)
        action(ACT_SELECT);
      else
        refresh();
    } else {
      game_touch(&g, world_x(e->x), world_y(e->y), 2);
      save();
      refresh();
    }
  }
}
static void focus(bool active) {
  if (!canvas || exiting) return;
  focused = active;
  touching = replay_select = false;
  if (!active) {
    game_input(&g, ACT_RELEASE_UP);
    game_input(&g, ACT_RELEASE_DOWN);
    game_input(&g, ACT_RELEASE_SELECT);
    save();
  }
  schedule();
}
static void init(void) {
  win = window_create();
  window_set_background_color(win, GColorBlack);
  GRect bounds = layer_get_bounds(window_get_root_layer(win));
  view_w = bounds.size.w;
  view_h = bounds.size.h;
  field_x = (view_w - scale(FIELD_W)) / 2;
  field_y = (view_h - scale(FIELD_H)) / 2;
  canvas = layer_create(bounds);
  layer_set_update_proc(canvas, draw);
  layer_add_child(window_get_root_layer(win), canvas);
  menu = menu_layer_create(GRect(
      PBL_IF_ROUND_ELSE(25, 8), PBL_IF_ROUND_ELSE(30, 20),
      view_w - PBL_IF_ROUND_ELSE(50, 16), view_h - PBL_IF_ROUND_ELSE(60, 40)));
  menu_layer_set_callbacks(menu, NULL,
                           (MenuLayerCallbacks){.get_num_rows = row_count,
                                                .get_cell_height = row_height,
                                                .draw_row = draw_row,
                                                .select_click = menu_select});
  menu_layer_set_center_focused(menu, false);
  layer_set_hidden(menu_layer_get_layer(menu), true);
  menu_layer_set_normal_colors(menu, GColorBlack, GColorWhite);
  menu_layer_set_highlight_colors(menu, GColorWhite, GColorBlack);
  layer_add_child(window_get_root_layer(win), menu_layer_get_layer(menu));
  if (!save_load(&io, &g))
    new_game();
  game_input(&g, ACT_RELEASE_UP);
  game_input(&g, ACT_RELEASE_DOWN);
  game_input(&g, ACT_RELEASE_SELECT);
  if (persist_read_int(51))
    screen = 0;
  WatchInfoVersion v = watch_info_get_firmware_version();
  touch_on = v.major > 4 || (v.major == 4 &&
                             (v.minor > 33 || (v.minor == 33 && v.patch >= 2)));
  if (touch_on) {
    app_touch_navigation_enable(false);
    window_set_touch_bridge_disabled(win, true);
    touch_service_subscribe(touch, NULL);
  }
  app_focus_service_subscribe(focus);
  window_stack_push(win, false);
  save();
  refresh();
}
static void deinit(void) {
  exiting = true;
  if (exit_timer) app_timer_cancel(exit_timer);
  if (replay_timer)
    app_timer_cancel(replay_timer);
  if (timer)
    app_timer_cancel(timer);
  save();
  if (touch_on)
    touch_service_unsubscribe();
  app_focus_service_unsubscribe();
  menu_layer_destroy(menu);
  menu = NULL;
  layer_destroy(canvas);
  canvas = NULL;
  window_destroy(win);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}
