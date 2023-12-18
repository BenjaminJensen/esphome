#include "esphome/core/log.h"
#include "float_relay_ladder.h"
#include <driver/gpio.h>

namespace esphome {
namespace float_relay_ladder {

/*************************************************
 * Constants
*************************************************/
static const char *TAG = "float_relay_ladder.output";

static const gpio_num_t RESISTOR_RELAY5 = GPIO_NUM_27;
static const gpio_num_t RESISTOR_RELAY4 = GPIO_NUM_26;
static const gpio_num_t RESISTOR_RELAY3 = GPIO_NUM_25;
static const gpio_num_t RESISTOR_RELAY2 = GPIO_NUM_33;
static const gpio_num_t RESISTOR_RELAY1 = GPIO_NUM_32;
static const gpio_num_t RESISTOR_RELAY0 = GPIO_NUM_22;
static const gpio_num_t RESISTOR_RELAY_ = GPIO_NUM_21;

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<RESISTOR_RELAY0) | (1ULL<<RESISTOR_RELAY1) | (1ULL<<RESISTOR_RELAY2) | (1ULL<<RESISTOR_RELAY3) | (1ULL<<RESISTOR_RELAY4) | (1ULL<<RESISTOR_RELAY5) )

/*************************************************
 * Public functions
*************************************************/

void FloatRelayLadder::setup(){
    // Configure GPIO for resistor ladder
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

void FloatRelayLadder::write_state(float state){
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

    ESP_LOGI(TAG, "Fan speed: %f", state);
}

void FloatRelayLadder::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty custom float output");
}

} //namespace empty_float_output
} //namespace esphome