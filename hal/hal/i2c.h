#pragma once

#include <stdint.h>

typedef enum i2c_port_t {
  I2C_PORT_1 = 0x0,
  I2C_PORT_2 = 0x1,
} i2c_port_t;

typedef struct i2c_device_t {
  i2c_port_t port;
  uint8_t address;
} i2c_device_t;

void i2c_init(i2c_port_t port);

void i2c_enable(i2c_port_t port);

void i2c_disable(i2c_port_t port);

uint8_t i2c_read(i2c_device_t dev, uint8_t dev_reg, uint8_t n, uint8_t* data);

uint8_t i2c_write(i2c_device_t dev, uint8_t dev_reg, uint8_t n, uint8_t* data);

uint8_t i2c_send_command(i2c_device_t dev, uint8_t cmd);
