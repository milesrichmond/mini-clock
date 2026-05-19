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

#include "display.h"
#include "i2c.h"
#include "sht45.h"
#include "util.h"

#include <stdint.h>
#include <string.h>

#include <driver/i2c_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const i2c_device_config_t display_1_i2c_config = {
    .device_address = 0x70,
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .scl_speed_hz = 100'000,
};
static const i2c_device_config_t display_2_i2c_config = {
    .device_address = 0x71,
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .scl_speed_hz = 100'000,
};
static const i2c_device_config_t sht45_i2c_config = {
    .device_address = 0x44,
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .scl_speed_hz = 100'000,
};

static display_t display_1 = {
    .config =
        {
            .blink_speed = DISPLAY_BLINK_STATIC,
            .en_display = true,
            .en_oscillator = true,
            .brightness = 0x4,
            .update_time_ms = 5000,
        },
    .animation =
        {
            .rtl = false,
            .is_static = true,
            .frame = 0,
        },
};
static display_t display_2 = {
    .config =
        {
            .blink_speed = DISPLAY_BLINK_STATIC,
            .en_display = true,
            .en_oscillator = true,
            .brightness = 0x4,
            .update_time_ms = 5000,
        },
    .animation =
        {
            .rtl = false,
            .is_static = true,
            .frame = 0,
        },
};
static sht45_t sht45 = {
    .config =
        {
            .fahrenheit = true,
            .update_time_ms = 2500,
        },
    .rh = 0,
    .temp = 0,
};

void app_main(void) {
  const TickType_t maxBlockTime = pdMS_TO_TICKS(10'000);
  uint32_t notify;

  rolling_average_t temp = {0};
  rolling_average_t humidity = {0};

  // I2C init
  init_i2c();
  display_1.i2c_handle = init_i2c_dev(0, &display_1_i2c_config);
  display_2.i2c_handle = init_i2c_dev(0, &display_2_i2c_config);
  sht45.i2c_handle = init_i2c_dev(0, &sht45_i2c_config);

  // Task init
  sht45.parent_task = xTaskGetCurrentTaskHandle();
  sht45_init(&sht45);
  display_init(&display_1);
  display_init(&display_2);

  // Main loop
  for (;;) {
    // Wait for notification
    if (pdPASS == xTaskNotifyWait(pdFALSE, ULONG_MAX, &notify, maxBlockTime)) {
      // sht45 update -> update display data
      if ((notify & SHT_NOTIFY_UPDATE) != 0) {
        rolling_average_update(&temp, sht45.temp);
        rolling_average_update(&humidity, sht45.rh);

        snprintf(display_1.animation.data, 32, "%4.2f", temp.average);
        snprintf(display_2.animation.data, 32, "%4.2f", humidity.average);

        xTaskNotify(display_1.task, DISPLAY_NOTIFY_ANIMATION, eSetBits);
        xTaskNotify(display_2.task, DISPLAY_NOTIFY_ANIMATION, eSetBits);
      }
    } else {
      // uh oh, no responses from device tasks
      esp_system_abort("Device tasks did not update in time\n");
    }
  }
}
