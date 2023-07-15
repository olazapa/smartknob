#if SK_DISPLAY
#include "display_task.h"
#include "semaphore_guard.h"

LV_FONT_DECLARE(roboto_light_60);
LV_FONT_DECLARE(roboto_light_24);

static const uint8_t LEDC_CHANNEL_LCD_BACKLIGHT = 0;

TFT_eSPI tft_ = TFT_eSPI();

static lv_disp_draw_buf_t disp_buf;
static lv_point_t points_left_bound[2] = {{0,0}, {0,0}};
static lv_point_t points_right_bound[2] = {{0,0}, {0,0}};
static lv_color_t FILL_COLOR = lv_color_make(90, 18, 151);
static lv_color_t DOT_COLOR = lv_color_make(80, 100, 200);

const int RADIUS = TFT_WIDTH / 2;

DisplayTask::DisplayTask(const uint8_t task_core) : Task{"Display", 4096, 1, task_core} {
  knob_state_queue_ = xQueueCreate(1, sizeof(PB_SmartKnobState));
  assert(knob_state_queue_ != NULL);

  mutex_ = xSemaphoreCreateMutex();
  assert(mutex_ != NULL);
}

DisplayTask::~DisplayTask() {
  vQueueDelete(knob_state_queue_);
  vSemaphoreDelete(mutex_);
}

void disp_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft_.startWrite();
    tft_.setAddrWindow(area->x1, area->y1, w, h);
    tft_.setSwapBytes(true);
    tft_.pushPixels(&color_p->full, w * h);
    tft_.endWrite();

    lv_disp_flush_ready(disp);
}

void DisplayTask::run() {    
    tft_.begin();
    tft_.invertDisplay(1);
    tft_.setRotation(SK_DISPLAY_ROTATION);
    tft_.fillScreen(TFT_DARKGREEN);

    ledcSetup(LEDC_CHANNEL_LCD_BACKLIGHT, 5000, 12);
    ledcAttachPin(PIN_LCD_BACKLIGHT, LEDC_CHANNEL_LCD_BACKLIGHT);
    ledcWrite(LEDC_CHANNEL_LCD_BACKLIGHT, UINT16_MAX);

    lv_init();
    lv_color_t *buf = (lv_color_t*) heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf);
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
  
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
      
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
  
    screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);       
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(screen, FILL_COLOR, LV_PART_MAIN);   
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);   

    static lv_style_t style_line; 
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_white());
    lv_style_set_line_rounded(&style_line, true);

    line_left_bound = lv_line_create(screen);
    lv_obj_add_style(line_left_bound,&style_line, 0);
    lv_obj_set_pos(line_left_bound, 0, 0);

    line_right_bound = lv_line_create(screen);
    lv_obj_add_style(line_right_bound,&style_line, 0);
    lv_obj_set_pos(line_right_bound, 0, 0);

    arc_dot = lv_arc_create(screen);
    lv_obj_set_size(arc_dot, TFT_WIDTH, TFT_HEIGHT);
    lv_obj_align(arc_dot, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(arc_dot, 10, LV_PART_MAIN);
    lv_arc_set_bg_angles(arc_dot, 0, 0); // Hides arc background by setting background arc angles to 0  
    lv_obj_remove_style(arc_dot, NULL, LV_PART_INDICATOR); // Hides arc indicator - for dot indicator we only want the knob part of the lvgl arc widget
    lv_obj_set_style_bg_color(arc_dot, DOT_COLOR, LV_PART_KNOB);
    lv_arc_set_rotation(arc_dot, 270);

    arc = lv_arc_create(screen);
    lv_obj_set_size(arc, TFT_WIDTH, TFT_HEIGHT);
    lv_obj_align(arc,LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(arc, 8, LV_PART_MAIN);
    lv_arc_set_bg_angles(arc, 0, 0); // Hides arc background by setting background arc angles to 0
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB); // Hides arc knob - for arc on the boundaries we only want the indicator part of the lvgl arc widget
    lv_obj_set_style_arc_color(arc, DOT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 5, LV_PART_INDICATOR);
    lv_arc_set_mode(arc, LV_ARC_MODE_REVERSE);

    label_cur_pos = lv_label_create(screen);
    lv_obj_set_style_text_align(label_cur_pos, LV_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label_cur_pos, LV_ALIGN_CENTER, 0, -VALUE_OFFSET);
    lv_label_set_text(label_cur_pos, "0");
    lv_obj_set_style_text_color(label_cur_pos, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_cur_pos, &roboto_light_60, 0);
    
    label_desc = lv_label_create(screen);
    lv_label_set_text(label_desc, "");
    lv_obj_set_style_text_align(label_desc, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_desc, LV_ALIGN_CENTER, 0, DESCRIPTION_Y_OFFSET);
    lv_obj_set_style_text_color(label_desc, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_desc, &roboto_light_24, LV_PART_MAIN);
    lv_obj_set_style_text_letter_space(label_desc, 1, LV_PART_MAIN);

    PB_SmartKnobState state;

    while(1) {
        if (xQueueReceive(knob_state_queue_, &state, portMAX_DELAY) == pdFALSE) {
          continue;
        }

        lv_label_set_text(label_desc, state.config.text);
        lv_obj_set_style_text_align(label_desc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_align(label_desc, LV_ALIGN_CENTER, 0, DESCRIPTION_Y_OFFSET); // Realigns label and its content to the center - this is required every time after setting text value  

        int32_t num_positions = state.config.max_position - state.config.min_position + 1;
        if (num_positions > 1) {
          int32_t fill_height = 255 - state.current_position * 255 / (num_positions - 1);
          lv_obj_set_style_bg_grad_stop(screen, fill_height, LV_PART_MAIN);
          lv_obj_set_style_bg_main_stop(screen, fill_height, LV_PART_MAIN); // Sets the same value for gradient and main - gives sharp line on gradient
        } else {
          lv_obj_set_style_bg_grad_stop(screen, 255, LV_PART_MAIN);
          lv_obj_set_style_bg_main_stop(screen, 255, LV_PART_MAIN);
        }

        lv_label_set_text_fmt(label_cur_pos,"%d" , state.current_position);
        lv_obj_set_style_text_align(label_cur_pos, LV_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_align(label_cur_pos, LV_ALIGN_CENTER, 0, -VALUE_OFFSET);

        float left_bound = PI / 2;
        float right_bound = PI / 2;
        
        if (num_positions > 0) {
          float range_radians = (state.config.max_position - state.config.min_position) * state.config.position_width_radians;
          left_bound = PI / 2 + range_radians / 2;
          right_bound = PI / 2 - range_radians / 2;
          points_left_bound[0] = {
                                  (lv_coord_t)(TFT_WIDTH/2 + RADIUS * cosf(left_bound)),
                                  (lv_coord_t)(TFT_HEIGHT/2 - RADIUS * sinf(left_bound))
                                };
          points_left_bound[1] = {
                                  (lv_coord_t)(TFT_WIDTH/2 + (RADIUS - 10) * cosf(left_bound)),
                                  (lv_coord_t)(TFT_HEIGHT/2 - (RADIUS - 10) * sinf(left_bound))
                                };
          lv_line_set_points(line_left_bound,points_left_bound,2);
          points_right_bound[0] = {
                                  (lv_coord_t)(TFT_WIDTH/2 + RADIUS * cosf(right_bound)),
                                  (lv_coord_t)(TFT_HEIGHT/2 - RADIUS * sinf(right_bound))
                                };
          points_right_bound[1] = {
                                  (lv_coord_t)(TFT_WIDTH/2 + (RADIUS - 10) * cosf(right_bound)),
                                  (lv_coord_t)(TFT_HEIGHT/2 - (RADIUS - 10) * sinf(right_bound))
                                };                           
          lv_line_set_points(line_right_bound, points_right_bound,2);
        }
        else {
          points_left_bound[0] = {0,0};
          points_left_bound[1] = {0,0};
          lv_line_set_points(line_left_bound, points_left_bound, 2);
          points_right_bound[0] = {0,0};
          points_right_bound[1] = {0,0};
          lv_line_set_points(line_right_bound, points_right_bound, 2);
        }
        
        
        float adjusted_sub_position = state.sub_position_unit * state.config.position_width_radians;
        if (num_positions > 0) {
          if (state.current_position == state.config.min_position && state.sub_position_unit < 0) {
            adjusted_sub_position = -logf(1 - state.sub_position_unit  * state.config.position_width_radians / 5 / PI * 180) * 5 * PI / 180;
          } else if (state.current_position == state.config.max_position && state.sub_position_unit > 0) {
            adjusted_sub_position = logf(1 + state.sub_position_unit  * state.config.position_width_radians / 5 / PI * 180)  * 5 * PI / 180;
          }
        }

        float raw_angle = left_bound - (state.current_position - state.config.min_position) * state.config.position_width_radians;
        float adjusted_angle = raw_angle - adjusted_sub_position;

        int32_t raw_angle_offset = (int) (-((raw_angle * (180/PI)) - 90));
        int32_t adjusted_angle_offset = (int) (-((adjusted_angle * (180/PI)) - 90));

        if (num_positions > 0 && ((state.current_position == state.config.min_position && state.sub_position_unit < 0) || (state.current_position == state.config.max_position && state.sub_position_unit > 0))) {
          lv_arc_set_angles(arc_dot, raw_angle_offset, raw_angle_offset);
          lv_obj_clear_flag(arc, LV_OBJ_FLAG_HIDDEN);
          if (raw_angle_offset < adjusted_angle_offset) {
            lv_arc_set_rotation(arc, 270 + left_bound * (180/PI));
            lv_arc_set_angles(arc, 270, 270 - (raw_angle_offset-adjusted_angle_offset));
          } else {
            lv_arc_set_rotation(arc, 270 + right_bound * (180/PI));
            lv_arc_set_angles(arc, 270 - (raw_angle_offset-adjusted_angle_offset), 270);
          }
        } else {
          lv_obj_add_flag(arc, LV_OBJ_FLAG_HIDDEN);
          lv_arc_set_angles(arc_dot, adjusted_angle_offset, adjusted_angle_offset);
        }

        lv_task_handler();

        {
          SemaphoreGuard lock(mutex_);
          ledcWrite(LEDC_CHANNEL_LCD_BACKLIGHT, brightness_);
        }
        delay(2);
    }
}

QueueHandle_t DisplayTask::getKnobStateQueue() {
  return knob_state_queue_;
}

void DisplayTask::setBrightness(uint16_t brightness) {
  SemaphoreGuard lock(mutex_);
  brightness_ = brightness;
}

void DisplayTask::setLogger(Logger* logger) {
    logger_ = logger;
}

void DisplayTask::log(const char* msg) {
    if (logger_ != nullptr) {
        logger_->log(msg);
    }
}

#endif