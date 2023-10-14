# LVAR 75 ESP Home controller

## Custom RPM sensor
Based on [David Antliff esp32-freqcount](https://github.com/DavidAntliff/esp32-freqcount)
Copyright (c) 2018 David Antliff
MIT License

*rpm_hw_sensor.h*
## Custom Output for fan speed
Resistor ladder controled by relays

*fan_speed.h*
## Docker commands
### Create
docker run --rm -v d:\git\esphome\genvex2:/config -it esphome/esphome wizard genvex2.yaml

### Compile
docker run --rm -v d:\git\esphome\genvex2:/config -it esphome/esphome compile genvex2.yaml

### run
DOES NOT WORK with com port
docker run --rm -v d:\git\esphome\genvex2:/config -it esphome/esphome run genvex2.yaml

### flash
install: python venv. python -m venv venv
Activate: D:\git\esphome\genvex2\venv\Scripts> .\activate
install esphomeflasher: https://github.com/esphome/esphome-flasher
Flash with: D:\git\esphome\genvex2\.esphome\build\gulvvarme\.pioenvs\gulvvarme\firmware.bin
