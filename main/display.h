#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <driver/i2c_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DISPLAY_NOTIFY_CONFIG (0x2)
#define DISPLAY_NOTIFY_ANIMATION (0x1)

typedef struct {
  uint16_t update_time_ms;

  bool en_display : 1;
  bool en_oscillator : 1;
  uint8_t brightness : 4;

  enum {
    DISPLAY_BLINK_STATIC = 0x0,
    DISPLAY_BLINK_05HZ = 0x3,
    DISPLAY_BLINK_1HZ = 0x2,
    DISPLAY_BLINK_2HZ = 0x1,
  } blink_speed : 2;
} display_config_t;

typedef struct {
  uint16_t period_ms;
  bool is_static : 1;
  bool rtl : 1;
  uint8_t frame : 6;
  char data[32];
} display_animation_t;

typedef struct {
  TaskHandle_t task;
  i2c_master_dev_handle_t i2c_handle;

  display_config_t config;
  display_animation_t animation;
} display_t;

void display_init(display_t *display);
