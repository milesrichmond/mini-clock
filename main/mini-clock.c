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

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <string.h>

#include "display.h"
#include "i2c.h"
#include "sht45.h"

static i2c_device_config_t display_1_conf = {
    .device_address = 0x71,
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .scl_speed_hz = 100000,
};
static disp_t display_1 = {
    .config =
        {
            .blink_speed = DISPLAY_BLINK_STATIC,
            .en_display = true,
            .en_oscillator = true,
            .brightness = 0x7,
        },
    .update_time_ms = 250,
    .update_config = true,
};

static i2c_device_config_t display_2_conf = {
    .device_address = 0x70,
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .scl_speed_hz = 100000,
};
static disp_t display_2 = {
    .config =
        {
            .blink_speed = DISPLAY_BLINK_STATIC,
            .en_display = true,
            .en_oscillator = true,
            .brightness = 0x7,
        },
    .update_time_ms = 250,
    .update_config = true,
};

static sht45_t sht45 = {
    .config.fahrenheit = true,
    .config.update_time_ms = 1000,
};

void app_main(void) {
  float temp_average = 0.0f;
  float rh_average = 0.0f;

  init_i2c();

  display_1.i2c_device = init_i2c_dev(0, &display_1_conf);
  display_2.i2c_device = init_i2c_dev(0, &display_2_conf);

  disp_init(&display_1);
  disp_init(&display_2);
  sht45_init(0, &sht45);

  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));

    /* Simple weighted average */
    temp_average = (0.95 * temp_average) + (0.05 * sht45.temp);
    rh_average = (0.95 * rh_average) + (0.05 * sht45.rh);

    printf("temp_average: %f\n", temp_average);
    printf("rh_average: %f\n\n", rh_average);

    snprintf(display_1.animation.str, 32, "%4.2f", rh_average);
    display_1.animation.length = strlen(display_1.animation.str) - 1;

    snprintf(display_2.animation.str, 32, "%4.2f", temp_average);
    display_2.animation.length = strlen(display_2.animation.str) - 1;
  }
}
