#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef struct {
  enum {
    DISPLAY_BLINK_STATIC = 0x0,
    DISPLAY_BLINK_05HZ = 0x3,
    DISPLAY_BLINK_1HZ = 0x2,
    DISPLAY_BLINK_2HZ = 0x1,
  } blink_speed : 3;
  uint8_t brightness : 4;

  bool en_display : 1;
  bool en_oscillator : 1;
} disp_config_t;

typedef struct {
  char str[32];
  uint8_t length;
} disp_animation_t;

/* I might create memory blocks for statically allocated tasks in the future */
typedef struct {
  TaskHandle_t task;
  i2c_master_dev_handle_t i2c_device;
  disp_config_t config;
  disp_animation_t animation;
  uint16_t update_time_ms;

  bool update_config : 1;
  bool reset_animation : 1;
} disp_t;

void disp_init(disp_t *const display);
