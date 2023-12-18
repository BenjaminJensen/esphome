#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace float_relay_ladder {

class FloatRelayLadder : public output::FloatOutput, public Component {
 public:
  void setup() override;
  void write_state(float state) override;
  void dump_config() override;
};


} //namespace float_relay_ladder
} //namespace esphome