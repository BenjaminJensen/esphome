#include "esphome/core/log.h"
#include "rpm_hw_sensor.h"

namespace esphome {
namespace rpm_hw_sensor {

/*************************************************
 * Constants
*************************************************/
static const char *TAG = "rpm_hw_sensor.sensor";
static const gpio_num_t rmt_gpio = GPIO_NUM_16;
static const int c_rmt_clk_div = 160;
static const uint16_t FILTER_LENGTH = 1023;  // APB @ 80MHz, limits to < 39,100 Hz
static const double c_sampling_window_seconds = 2.0;
static const double c_sampling_period_seconds = 2.0;
// assuming 80MHz APB clock
static const double rmt_period = (double)(c_rmt_clk_div) / 80000000.0;

/*************************************************
 * Public functions
*************************************************/

void RPMHwSensor::setup() {
    // This will be called by App.setup()
    init_rmt(rmt_gpio, RMT_CHANNEL_0, c_rmt_clk_div);
    
    // RPM 1
    init_pcnt(GPIO_NUM_34, rmt_gpio, PCNT_UNIT_0, PCNT_CHANNEL_0, FILTER_LENGTH);
    // RPM2
    init_pcnt(GPIO_NUM_35, rmt_gpio, PCNT_UNIT_1, PCNT_CHANNEL_0, FILTER_LENGTH);

    // Setup sampling window
    memset(rmt_items, 0, items_size*sizeof(rmt_item32_t));
    num_rmt_items = create_rmt_window(rmt_items, c_sampling_window_seconds, rmt_period);  
}

void RPMHwSensor::loop() {
  
}

void RPMHwSensor::update() {
    static uint8_t state = 0;
    bool done = false;

    switch(state)     {
      case 0:
        // setup run
        
        // clear counter
        ESP_ERROR_CHECK(pcnt_counter_clear(PCNT_UNIT_0));
        ESP_ERROR_CHECK(pcnt_counter_clear(PCNT_UNIT_1));

        // start sampling window
        ESP_ERROR_CHECK(rmt_write_items(RMT_CHANNEL_0, rmt_items, num_rmt_items, false));

        // next state
        state++;
        break;
      case 1:
        // wait

        // wait for window to finish
        //ESP_ERROR_CHECK(rmt_wait_tx_done(RMT_CHANNEL_0, portMAX_DELAY));
        if(rmt_wait_tx_done(RMT_CHANNEL_0, 0) == ESP_OK) {
          done = true;
          state = 0;
        }
        break;
      default:
        ESP_LOGW(TAG, "Something is wrong");
        break;
    }

    if( done == true) {
      // read counter
      int16_t count = 0;
      int16_t count2 = 0;
      ESP_ERROR_CHECK(pcnt_get_counter_value(PCNT_UNIT_0, &count));
      ESP_ERROR_CHECK(pcnt_get_counter_value(PCNT_UNIT_1, &count2));

      // TODO: check for overflow?

      double frequency_hz = count / 2.0 / c_sampling_window_seconds;
      double frequency_hz2 = count2 / 2.0 / c_sampling_window_seconds;

    if (this->rpm1_ != nullptr)
        this->rpm1_->publish_state(frequency_hz * 60);
    if (this->rpm2_ != nullptr)
        this->rpm2_->publish_state(frequency_hz2 * 60);
    }
}

void RPMHwSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "RPM HW sensor");
}

/*************************************************
 * Private functions
*************************************************/
void RPMHwSensor::init_rmt(gpio_num_t tx_gpio, rmt_channel_t channel, uint8_t clk_div) {
    // This will be called by App.setup()
    ESP_LOGD(TAG, "%s", __FUNCTION__);

    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = channel;
    rmt_tx.gpio_num = tx_gpio;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = clk_div;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_en = false;
    rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    rmt_tx.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&rmt_tx));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_tx.channel, 0, 0));
}

void RPMHwSensor::init_pcnt(gpio_num_t pulse_gpio, gpio_num_t ctrl_gpio, pcnt_unit_t unit, pcnt_channel_t channel, uint16_t filter_length) {
    ESP_LOGD(TAG, "%s", __FUNCTION__);

    // set up counter
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = pulse_gpio,
        .ctrl_gpio_num = ctrl_gpio,
        .lctrl_mode = PCNT_MODE_DISABLE,
        .hctrl_mode = PCNT_MODE_KEEP,
        .pos_mode = PCNT_COUNT_INC,  // count both rising and falling edges
        .neg_mode = PCNT_COUNT_INC,
        .counter_h_lim = 0,
        .counter_l_lim = 0,
        .unit = unit,
        .channel = channel,
    };

    ESP_ERROR_CHECK(pcnt_unit_config(&pcnt_config));

    // set the GPIO back to high-impedance, as pcnt_unit_config sets it as pull-up
    ESP_ERROR_CHECK(gpio_set_pull_mode(pulse_gpio, GPIO_FLOATING));

    // enable counter filter - at 80MHz APB CLK, 1000 pulses is max 80,000 Hz, so ignore pulses less than 12.5 us.
    ESP_ERROR_CHECK(pcnt_set_filter_value(unit, filter_length));
    ESP_ERROR_CHECK(pcnt_filter_enable(unit));
}

int RPMHwSensor::create_rmt_window(rmt_item32_t * items, double sampling_window_seconds, double rmt_period) {
    ESP_LOGD(TAG, "%s", __FUNCTION__);

    int num_items = 0;

    // enable counter for exactly x seconds:
    int32_t total_duration = (uint32_t)(sampling_window_seconds / rmt_period);
    ESP_LOGD(TAG, "total_duration %f seconds = %d * %g seconds", sampling_window_seconds, total_duration, rmt_period);

    // max duration per item is 2^15-1 = 32767
    while (total_duration > 0)
    {
      uint32_t duration = total_duration > 32767 ? 32767 : total_duration;
      items[num_items].level0 = 1;
      items[num_items].duration0 = duration;
      total_duration -= duration;
      ESP_LOGD(TAG, "duration %d", duration);

      if (total_duration > 0)
      {
          uint32_t duration = total_duration > 32767 ? 32767 : total_duration;
          items[num_items].level1 = 1;
          items[num_items].duration1 = duration;
          total_duration -= duration;
      }
      else
      {
          items[num_items].level1 = 0;
          items[num_items].duration1 = 0;
      }
      ESP_LOGD(TAG, "[%d].level0 %d", num_items, items[num_items].level0);
      ESP_LOGD(TAG, "[%d].duration0 %d", num_items, items[num_items].duration0);
      ESP_LOGD(TAG, "[%d].level1 %d", num_items, items[num_items].level1);
      ESP_LOGD(TAG, "[%d].duration1 %d", num_items, items[num_items].duration1);

      ++num_items;
    }
    ESP_LOGD(TAG, "num_items %d", num_items);
    return num_items;
}

} //namespace rpm_hw_sensor
} //namespace esphome