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

#include "sht45.h"

#include <stdbool.h>
#include <stdint.h>

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// DEFINES

#define TIMEOUT (500)

#define RAW_TO_FAHRENHEIT(x) (-49 + (315 * (x / 65535.0f)))
#define RAW_TO_CELCIUS(x) (-45 + (175 * (x / 65535.0f)))
#define RAW_TO_RH(x) (-6 + (125 * (x / 65535.0f)))

// TYPES

typedef enum sht45_command_t {
  SHT45_TRH_HIGH = 0xFD,
  SHT45_TRH_MED = 0xF6,
  SHT45_TRH_LOW = 0xE0,

  SHT45_SERIAL_NUM = 0x89,
  SHT45_RESET = 0x94,
} sht45_command_t;

// PROTOTYPES

esp_err_t read_data(const i2c_master_dev_handle_t handle, uint8_t *buf,
                    uint8_t buf_len);

// IMPLEMENTATION

void sht45_task(void *parameters) {
  sht45_t *const dev = (sht45_t *)parameters;
  esp_err_t err = ESP_OK;
  TickType_t lastWakeTime = xTaskGetTickCount();

  uint8_t read_buf[6];
  uint16_t staging_buf;

  for (;;) {
    // Read raw data. If there is an error here, the task will sleep and try
    // again later.
    err = read_data(dev->i2c_handle, read_buf, sizeof(read_buf));

    // Convert to human readable format
    if (err == ESP_OK) {
      staging_buf = ((uint16_t)read_buf[0] << 8) | (uint16_t)read_buf[1];
      if (dev->config.fahrenheit)
        dev->temp = RAW_TO_FAHRENHEIT(staging_buf);
      else
        dev->temp = RAW_TO_CELCIUS(staging_buf);

      staging_buf = ((uint16_t)read_buf[3] << 8) | (uint16_t)read_buf[4];
      dev->rh = RAW_TO_RH(staging_buf);

      xTaskNotify(dev->parent_task, 0x1, eSetBits); // Notify parent
    }

    // Sleep
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(dev->config.update_time_ms));
  }

  vTaskDelete(NULL);
}

void sht45_init(sht45_t *sht45) {
  if (pdFAIL == xTaskCreate(sht45_task, "sht45 Task", 4096, (void *)sht45, 1,
                            &sht45->task))
    esp_system_abort("Failed to create sht45 task!\n");
}

esp_err_t read_data(const i2c_master_dev_handle_t handle, uint8_t *buf,
                    uint8_t buf_len) {
  static const uint8_t command = SHT45_TRH_HIGH;
  esp_err_t error;

  error = i2c_master_transmit(handle, &command, 1, TIMEOUT);
  if (error != ESP_OK)
    return error;

  vTaskDelay(pdMS_TO_TICKS(10));

  error = i2c_master_receive(handle, buf, buf_len, TIMEOUT);
  return error;
}
