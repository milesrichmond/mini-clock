#pragma once

#include <hal/i2c.h>
#include <stdint.h>

typedef enum display_command_t {
    DISPLAY_DATA_ADDR = 0x00,
    DISPLAY_SYSTEM_SETUP = 0x20,
    DISPLAY_KEY_DATA = 0x40,
    DISPLAY_INT_FLAG = 0x60,
    DISPLAY_SETUP = 0x80,
    DISPLAY_ROW_INT = 0xA0,
    DISPLAY_DIMMING = 0xE0,
} display_command_t;

typedef enum display_config_t {
    DISPLAY_OFF = 0U,
    DISPLAY_ON = 1U,
    DISPLAY_BLINK_2HZ = 3U,
    DISPLAY_BLINK_1HZ = 5U,
    DISPLAY_BLINK_0_5HZ = 7U,
} display_config_t;

inline void display_enable(i2c_device_t dev) {
    i2c_send_command(dev, DISPLAY_SYSTEM_SETUP | 0x01);
}

inline void display_disable(i2c_device_t dev) {
    i2c_send_command(dev, DISPLAY_SYSTEM_SETUP | 0x00);
}

void display_config(i2c_device_t dev, display_config_t config, uint8_t duty);

void display_write(i2c_device_t dev, uint8_t bytes, uint8_t *data);
