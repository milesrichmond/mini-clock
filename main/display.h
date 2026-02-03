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
