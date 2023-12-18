# ESPHome Devices and components

## Building and installing
Create Python venv
Follow ESPHome installation guide (Do not run the docker version on windows as it is extreamly slow!)

### Compile
esphome compile DEVICE.yaml

### Compile & Run
esphome run DEVICE.yaml --device {COM6, DEVICE.local, IP}

### Adding existing device to Home Assistant
If you have build a device locally on you machine but would like to add it to your running HA instance, follow the steps below. It is not completly intuitiv but it works.

1. Navigate to the ESPHome add-on
2. Click "NEW DEVICE" in bottom right corner
3. Do not choose the "ESPHome Web" use the HA add-on
4. For name use the same name as you have in your local .yaml
5. Select the same "device type" as your local .yaml
6. At the "Configuration crreated!" screen, click "SKIP"
The device is now added, it is time to set it up:
7. Go to "EDIT" on the device on the dashboard
8. Paist the content of your local .yaml into the add-on editor in HA
9. Press "SAVE
10. Press "INSTALL"
11. Choose "WIRELESSLY" and it should automatically compile and run on DEVICE.local

After combilation all should be running.
If you changed the "API" key HA will ask you to reconfigure the device:
1. Go to "Settings->Devices & services" the device should be present with a red button saying "Reconfigure"
2. Push "Reconfigure" 
3. Add new "API" key
The device should now be working 
