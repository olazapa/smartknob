#pragma once

#include "lvgl.h"
#include "../proto_gen/smartknob.pb.h"

typedef enum {
    VIEW_CIRCLE_MENU          = 0,
    VIEW_LIST_MENU,
    VIEW_DIAL
} view_t;

class View {
    public:
        View() {}
        virtual ~View(){}

        virtual void setupView(PB_SmartKnobConfig config) = 0;
        virtual void updateView(PB_SmartKnobState state) = 0;
};