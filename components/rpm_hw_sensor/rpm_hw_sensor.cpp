#include "esphome/core/log.h"
#include "rpm_hw_sensor.h"

namespace esphome {
namespace rpm_hw_sensor {

static const char *TAG = "rpm_hw_sensor.sensor";

void RPMHwSensor::setup() {
  
}

void RPMHwSensor::loop() {
  
}

void RPMHwSensor::update() {
    if (this->rpm1_ != nullptr)
      this->rpm1_->publish_state(1.0f);
    if (this->rpm2_ != nullptr)
      this->rpm2_->publish_state(2.0f);
}

void RPMHwSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "RPM HW sensor");
}

} //namespace rpm_hw_sensor
} //namespace esphome