#include "esphome.h"
#include <driver/gpio.h>
using namespace esphome;

static const char *const TAG_FAN = "FAN_SPEED";

// GPIO's for relay outputs
/*
#define RESISTOR_RELAY5     (27)
#define RESISTOR_RELAY4     (26)
#define RESISTOR_RELAY3     (25)
#define RESISTOR_RELAY2     (33)
#define RESISTOR_RELAY1     (32)
#define RESISTOR_RELAY0     (22)
#define RESISTOR_RELAY_     (21)
*/
static const gpio_num_t RESISTOR_RELAY5 = GPIO_NUM_27;
static const gpio_num_t RESISTOR_RELAY4 = GPIO_NUM_26;
static const gpio_num_t RESISTOR_RELAY3 = GPIO_NUM_25;
static const gpio_num_t RESISTOR_RELAY2 = GPIO_NUM_33;
static const gpio_num_t RESISTOR_RELAY1 = GPIO_NUM_32;
static const gpio_num_t RESISTOR_RELAY0 = GPIO_NUM_22;
static const gpio_num_t RESISTOR_RELAY_ = GPIO_NUM_21;


#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<RESISTOR_RELAY0) | (1ULL<<RESISTOR_RELAY1) | (1ULL<<RESISTOR_RELAY2) | (1ULL<<RESISTOR_RELAY3) | (1ULL<<RESISTOR_RELAY4) | (1ULL<<RESISTOR_RELAY5) )


class FanSpeedOutput : public Component, public FloatOutput {
 public:
  void setup() override {
    /*
    #define RESISTOR_RELAY5     (27)
    #define RESISTOR_RELAY4     (26)
    #define RESISTOR_RELAY3     (25)
    #define RESISTOR_RELAY2     (33)
    #define RESISTOR_RELAY1     (32)
    #define RESISTOR_RELAY0     (22)
    #define RESISTOR_RELAY_     (21)
    
    */
    // This will be called by App.setup()
    // Setup OUTPUT
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
  }

  void write_state(float state) override {

    // Reset relays
    gpio_set_level(RESISTOR_RELAY0, 0);
    gpio_set_level(RESISTOR_RELAY1, 0);
    gpio_set_level(RESISTOR_RELAY2, 0);
    gpio_set_level(RESISTOR_RELAY3, 0);
    gpio_set_level(RESISTOR_RELAY4, 0);
    gpio_set_level(RESISTOR_RELAY5, 0);

    int level = 0;
    if(state >= 1.0f) {
        level = 5;
    }
    else if(state >= 0.8f) {
        level = 4;
    }
    else if(state >= 0.6f) {
        level = 3;
    }
    else if(state >= 0.4f) {
        level = 2;
    }
    else if(state >= 0.2f) {
        level = 1;
    }
    else {
        level = 0;
    }

    switch(level) {
        case 0:
            gpio_set_level(RESISTOR_RELAY0, 1);
            break;
        case 1:
            gpio_set_level(RESISTOR_RELAY1, 1);
            break;
        case 2:
            gpio_set_level(RESISTOR_RELAY2, 1);
            break;
        case 3:
            gpio_set_level(RESISTOR_RELAY3, 1);
            break;
        case 4:
            gpio_set_level(RESISTOR_RELAY4, 1);
            break;
        case 5:
            gpio_set_level(RESISTOR_RELAY5, 1);
            break;
        default:
            ESP_LOGW(TAG, "set_fan_on: level not supported (%d)", level);
            break;
    }

    ESP_LOGI(TAG_FAN, "Fan speed: %f", state);
    // state is the amount this output should be on, from 0.0 to 1.0
    // we need to convert it to an integer first
    //int value = state * 1024;
    //analogWrite(5, value);
  }
};