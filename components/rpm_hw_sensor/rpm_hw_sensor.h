#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <stdint.h>
#include <driver/rmt.h>
#include <driver/pcnt.h>

namespace esphome {
namespace rpm_hw_sensor {

static const size_t items_size = ((32) * 4) * 2;

class RPMHwSensor : public sensor::Sensor, public PollingComponent {
    public:  
        void set_rpm1(sensor::Sensor *rpm1) { rpm1_ = rpm1; }
        void set_rpm2(sensor::Sensor *rpm2) { rpm2_ = rpm2; }
        float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

        void setup() override;
        void loop() override;
        void update() override;
        void dump_config() override;
    protected:
        sensor::Sensor *rpm1_;
        sensor::Sensor *rpm2_;

    private:
        rmt_item32_t rmt_items[items_size];
        int num_rmt_items;
        void init_rmt(gpio_num_t tx_gpio, rmt_channel_t channel, uint8_t clk_div);
        void init_pcnt(gpio_num_t pulse_gpio, gpio_num_t ctrl_gpio, pcnt_unit_t unit, pcnt_channel_t channel, uint16_t filter_length);
        int create_rmt_window(rmt_item32_t * items, double sampling_window_seconds, double rmt_period);
};

} //namespace rpm_hw_sensor
} //namespace esphome