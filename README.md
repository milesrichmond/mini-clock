# Mini-Clock

A simple clock + humidity & temperature sensor built ontop an Adafruit ESP32 Feather V2.  
Periodic sensor readings and display writes are individually managed by their own task. Sensor readings are filtered by a
weighted average to smooth out the effects of sudden air currents.

**NOTE:** Currently, there is no clock capability. I have not acquired an external clock module.

## Parts List

- 1x [Adafruit ESP32 Feather V2](https://www.adafruit.com/product/5900)
- 2x [Adafruit 0.56" 4-Digit 7-Segment Display w/I2C Backpack](https://www.adafruit.com/product/1002)
- 1x [Adafruit Sensirion SHT45](https://www.adafruit.com/product/5665)
- 1x Some external timekeeper (TBD)

## Assembly

All I2C devices are configured to be run on port 0, which itself is configured to use GPIO pin 20 as SCL and GPIO pin 22 as SDA.
On the Feather V2, these are simply labeled SCL and SDA respectively. During development, I also used external pull-up resistors
for both lines, but they are not neccessary; the microcontroller's internal pull-up resistors are enabled.

## Build instructions

First, ensure you have Espressif's esp-idf installed. Esp-idf is used for building and flashing the project
onto the microcontroller. Once installed, the build process is straightforward.

```
idf.py build
idf.py -p [port/device] flash

# Optionally, if there are any issues to debug
idf.py -p [port/device] monitor
```

I am unsure of how usb devices interface on other operating systems, you should refer to Espressif's own documentation.
On macos, the device to flash to appears as "/dev/cu.usbserial-...".
