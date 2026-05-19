//
// Copyright 2026 Miles Richmond
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

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
