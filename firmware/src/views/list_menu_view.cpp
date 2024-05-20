#include "list_menu_view.h"
#include "styles.h"
#include "view_util.h"

void ListMenuView::setup_list_elements(PB_SmartKnobConfig config) {
  lv_obj_set_scrollbar_mode(screen_, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(screen_, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen_, lv_color_black(), LV_PART_MAIN);
  list = lv_list_create(screen_);
  lv_obj_align(list, LV_ALIGN_CENTER, 0, button_size+30);
  lv_obj_set_width(list, TFT_WIDTH);
  lv_obj_set_style_bg_color(list, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
  setup_circle_button(&buttons[0], 0, config, display_task_->button_event_cb);
  for (int i=1; i < config.max_position + 1; i++) {
    setup_list_button(&buttons[i], i, config);
  }
}

void ListMenuView::setup_list_button(lv_obj_t ** button, int index, PB_SmartKnobConfig config) {
  *button = lv_btn_create(list);
  lv_obj_set_width(*button, lv_pct(100));
  lv_obj_t * label = lv_label_create(*button);
  if (index == 0) {
    lv_obj_set_style_text_font(label, &mdi_40, LV_PART_MAIN);
    lv_label_set_text(label, config.view_config.menu_entries[index].icon);
  } else {
    lv_obj_set_style_text_font(label, &roboto_light_24, LV_PART_MAIN);
    lv_label_set_text(label, config.view_config.menu_entries[index].description);
  }
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_style(*button, &list_btn_style, 0);
  lv_obj_add_style(*button, &list_btn_highlighted_style, LV_STATE_FOCUSED);

  lv_obj_set_user_data(*button, (void*) index);
  lv_obj_add_event_cb(*button, display_task_->button_event_cb, LV_EVENT_PRESSED, NULL);
}

void ListMenuView::setupView(PB_SmartKnobConfig config) {
  setup_list_elements(config);
}

void ListMenuView::updateView(PB_SmartKnobState state) {
  if (lv_obj_is_valid(buttons[0]) && state.current_position >= 0 && state.current_position <= state.config.max_position) {
    for (int i=0; i < state.config.max_position + 1; i++) {
      lv_obj_clear_state(buttons[i], LV_STATE_FOCUSED);
    }
    lv_obj_add_state(buttons[state.current_position], LV_STATE_FOCUSED);
  }
}