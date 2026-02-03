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
