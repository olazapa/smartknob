#pragma once

#include <Arduino.h>
#include "task.h"
#include "proto_gen/smartknob.pb.h"

struct Message {
    String trigger_name;
    int trigger_value;
};

class ConnectivityTask : public Task<ConnectivityTask> {
    friend class Task<ConnectivityTask>; // Allow base Task to invoke protected run()

    public:
        ConnectivityTask(const uint8_t task_core);
        ~ConnectivityTask();
        void setConfigCallback(PB_SmartKnobConfig &callback);

        void sendMqttMessage(Message message);
        void setConfigCallback(std::function<void(PB_SmartKnobConfig&)> callback);
        
    protected:
        void run();

    private:
        QueueHandle_t queue_;
        std::function<void(PB_SmartKnobConfig&)> configCallback_;

        void initWiFi();
        void connectToMqttBroker();
};
