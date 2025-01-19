
#include "page.h"
#include "views/view.h"
#include "../motor_task.h"
#include "../connectivity_task.h"
#include "interface_callbacks.h"
#include "proto_gen/smartknob.pb.h"

typedef std::function<void(PB_SmartKnobConfig&)> ConfigCallback;

class LightsPage : public Page {
    public:
        LightsPage(ConnectivityTask& connectivity_task, ConfigCallback config_callback);
        ~LightsPage(){}

        PB_SmartKnobConfig * getPageConfig() override;
        void handleState(PB_SmartKnobState state) override;
        void handleUserInput(input_t input, int input_data, PB_SmartKnobState state) override;
    
    private:
        int current_config_ = 0;

        ConnectivityTask& connectivity_task_;
        ConfigCallback config_callback_;

        uint32_t last_publish_time;
        uint32_t last_published_position;
        
        PB_SmartKnobConfig configs_[2] =
        {
            {
                .has_view_config = true,
                .view_config = 
                {
                    VIEW_DIAL,
                    "Desk lights\nbrightness"
                },
                .position = 0,
                .sub_position_unit = 0,
                .position_nonce = 6,
                .min_position = 0,
                .max_position = 100,
                .infinite_scroll = false,
                .position_width_radians = 2 * PI / 180,
                .detent_strength_unit = 0.2,
                .endstop_strength_unit = 1,
                .snap_point = 1.1,
                .detent_positions_count = 0,
                .detent_positions = {},
                .snap_point_bias = 0,
                .led_hue = 30
            },
            {
                .has_view_config = true,
                .view_config = 
                {
                    VIEW_DIAL,
                    "Desk lights\ntemperature"
                },
                .position = 0,
                .sub_position_unit = 0,
                .position_nonce = 6,
                .min_position = 0,
                .max_position = 100,
                .infinite_scroll = false,
                .position_width_radians = 2 * PI / 180,
                .detent_strength_unit = 0.2,
                .endstop_strength_unit = 1,
                .snap_point = 1.1,
                .detent_positions_count = 0,
                .detent_positions = {},
                .snap_point_bias = 0,
                .led_hue = 30
            }
        };
    
};