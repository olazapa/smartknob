#include "styles.h"

lv_style_t circle_btn_style;
lv_style_t circle_btn_highlighted_style;
lv_style_t list_btn_style;
lv_style_t list_btn_highlighted_style;
lv_style_t style_line;

void init_styles() {
    lv_style_transition_dsc_init(&trans_pr, props, lv_anim_path_linear, 0, 0, NULL);

    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_white());
    lv_style_set_line_rounded(&style_line, true);

    lv_style_init(&circle_btn_style);
    lv_style_set_shadow_width(&circle_btn_style, 0);
    lv_style_set_radius(&circle_btn_style, button_size);
    lv_style_set_bg_color(&circle_btn_style, lv_color_white());
    lv_style_set_bg_opa(&circle_btn_style, 0);
    lv_style_set_text_color(&circle_btn_style,lv_color_white());

    lv_style_init(&circle_btn_highlighted_style);
    lv_style_set_shadow_width(&circle_btn_highlighted_style, 0);
    lv_style_set_bg_color(&circle_btn_highlighted_style, lv_color_white());
    lv_style_set_bg_opa(&circle_btn_highlighted_style, 255);
    lv_style_set_transition(&circle_btn_highlighted_style, NULL);
    lv_style_set_text_color(&circle_btn_highlighted_style, lv_color_black());
    lv_style_set_transition(&circle_btn_highlighted_style, &trans_pr);

    lv_style_init(&list_btn_style);
    lv_style_set_shadow_width(&list_btn_style, 0);
    lv_style_set_bg_color(&list_btn_style, lv_color_white());
    lv_style_set_bg_opa(&list_btn_style, 0);
    lv_style_set_text_color(&list_btn_style,lv_color_white());

    lv_style_init(&list_btn_highlighted_style);
    lv_style_set_shadow_width(&list_btn_highlighted_style, 0);
    lv_style_set_bg_color(&list_btn_highlighted_style, lv_color_white());
    lv_style_set_bg_opa(&list_btn_highlighted_style, 255);
    lv_style_set_transition(&list_btn_highlighted_style, NULL);
    lv_style_set_text_color(&list_btn_highlighted_style, lv_color_black());
    lv_style_set_transition(&list_btn_highlighted_style, &trans_pr);
}