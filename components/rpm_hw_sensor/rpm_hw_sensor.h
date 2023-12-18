#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace rpm_hw_sensor {

class RPMHwSensor : public sensor::Sensor, public PollingComponent {
    public:  
        void set_rpm1(sensor::Sensor *rpm1) { rpm1_ = rpm1; }
        void set_rpm2(sensor::Sensor *rpm2) { rpm2_ = rpm2; }
        

        void setup() override;
        void loop() override;
        void update() override;
        void dump_config() override;
    protected:
        sensor::Sensor *rpm1_;
        sensor::Sensor *rpm2_;
};

} //namespace rpm_hw_sensor
} //namespace esphome