#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef struct {
  TaskHandle_t task;
  i2c_master_dev_handle_t i2c_handle;
  struct {
    uint16_t update_time_ms;
    bool fahrenheit;
  } config;
  float temp;
  float rh;
} sht45_t;

void sht45_init(uint8_t bus, sht45_t *const sht45);
