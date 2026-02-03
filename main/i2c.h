#pragma once

#include <stdint.h>

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_err.h>

#define I2C_STD_TIMEOUT (1000)

i2c_master_bus_handle_t init_i2c();

i2c_master_dev_handle_t init_i2c_dev(uint8_t bus,
                                     const i2c_device_config_t *dev);
