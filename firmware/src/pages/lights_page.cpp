#include "lights_page.h"

LightsPage::LightsPage(ConnectivityTask& connectivity_task, ConfigCallback config_callback)
    : Page(), connectivity_task_(connectivity_task), config_callback_(config_callback),
      last_published_position(-1), last_publish_time(0) {}

PB_SmartKnobConfig * LightsPage::getPageConfig() {
    return &configs_[0];
}

void LightsPage::handleState(PB_SmartKnobState state) {
    if (last_published_position != state.current_position) {
        if (millis() - last_publish_time > 500) {
            int current_position = state.current_position;
            Message msg;
            switch(current_config_)
            {
            case 0:
            {
                msg = {
                    .trigger_name = "lights_b",
                    .trigger_value = current_position
                };
                break;
            }
            case 1:
            {
                msg = {
                    .trigger_name = "lights_t",
                    .trigger_value = current_position
                };
                break;
            }
            default:
                break;
            }
            connectivity_task_.sendMqttMessage(msg);
            last_publish_time = millis();
            last_published_position = state.current_position;
        }
    }
}

void LightsPage::handleUserInput(input_t input, int input_data, PB_SmartKnobState state) {    
    switch (input)
    {
    case INPUT_BACK:
    {
        current_config_ = 0;
        if (page_change_callback_) {
            page_change_callback_(MAIN_MENU_PAGE);
        }
        break;
    }
    case INPUT_FORWARD:
    {
        current_config_++;
        if (current_config_ < sizeof(configs_) / sizeof(configs_[0])) {
            config_callback_(configs_[current_config_]);
        } else {
            current_config_ = 0;
            config_callback_(configs_[current_config_]);
        }
        break;
    }    
    default:
        break;
    }
}