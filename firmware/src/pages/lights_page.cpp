#include "lights_page.h"

PB_SmartKnobConfig * LightsPage::getPageConfig() {
    return &config_;
}

void LightsPage::handleState(PB_SmartKnobState state) {
    // handle state here
}

void LightsPage::handleUserInput(input_t input, int input_data, PB_SmartKnobState state) {    
    switch (input)
    {
    case INPUT_BACK:
    {
        if (page_change_callback_) {
            page_change_callback_(MAIN_MENU_PAGE);
        }
        break;
    }
    case INPUT_FORWARD:
    {
        if (page_change_callback_) {
            page_change_callback_(MAIN_MENU_PAGE);
        }
        break;
    }    
    default:
        break;
    }
}