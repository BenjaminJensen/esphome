import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import  CONF_ID, UNIT_REVOLUTIONS_PER_MINUTE , ICON_FAN 

empty_compound_sensor_ns = cg.esphome_ns.namespace('rpm_hw_sensor')
EmptyCompoundSensor = empty_compound_sensor_ns.class_('RPMHwSensor', cg.PollingComponent)

CONF_RPM1 = "rpm1"
CONF_RPM2 = "rpm2"


CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(EmptyCompoundSensor),
    cv.Optional(CONF_RPM1):
        sensor.sensor_schema(
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE , 
            accuracy_decimals=0, 
            icon=ICON_FAN).extend(),
    cv.Optional(CONF_RPM2):
        sensor.sensor_schema(
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE , 
            accuracy_decimals=0, 
            icon=ICON_FAN).extend(),
}).extend(cv.polling_component_schema('0.001s'))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    if CONF_RPM1 in config:
        conf = config[CONF_RPM1]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_rpm1(sens))
        
    if CONF_RPM2 in config:
        conf = config[CONF_RPM2]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_rpm2(sens))
        
        