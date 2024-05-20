#pragma once

#include "lvgl.h"

LV_FONT_DECLARE(roboto_light_60);
LV_FONT_DECLARE(roboto_light_24);
LV_FONT_DECLARE(mdi_40);

static lv_point_t points_left_bound[2] = {{0,0}, {0,0}};
static lv_point_t points_right_bound[2] = {{0,0}, {0,0}};
static lv_color_t FILL_COLOR = lv_color_make(242, 138, 23);
static lv_color_t DOT_COLOR = lv_color_make(252, 138, 23);

extern lv_style_t circle_btn_style;
extern lv_style_t circle_btn_highlighted_style;
extern lv_style_t list_btn_style;
extern lv_style_t list_btn_highlighted_style;
extern lv_style_t style_line;

static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, LV_STYLE_BG_OPA, (lv_style_prop_t) 0};
static lv_style_transition_dsc_t trans_pr;

const int button_size = 45;
const int button_padding = 7;
const int arc_dot_size = 10;
const int arc_dot_menu_padding = 61;
const int arc_dot_dial_padding = 7;

void init_styles();

