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
#include "util.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// DEFINES

#define TIMEOUT (1000)

// PROTOTYPES

esp_err_t update_config(const display_t *display);

esp_err_t update_animation(display_t *display);

// IMPLEMENTATION

void display_task(void *parameters) {
  BaseType_t gotNotification;
  uint32_t notify = 0;
  display_t *const display = (display_t *)parameters;

  ESP_ERROR_CHECK(update_config(display));

  for (;;) {
    gotNotification = xTaskNotifyWait(pdFALSE, ULONG_MAX, &notify,
                                      display->config.update_time_ms);

    if (gotNotification == pdTRUE) {
      // Update for new display config
      if (notify & DISPLAY_NOTIFY_CONFIG)
        ESP_ERROR_CHECK(update_config(display));

      // Update for new display animation
      if (notify & DISPLAY_NOTIFY_ANIMATION) {
        display->animation.frame = 0;
        update_animation(display);
      }
    } else {
      // No notifications in update period.
      // Update if dynamic animation
      if (!display->animation.is_static)
        update_animation(display);
    }
  }

  vTaskDelete(NULL);
}

void display_init(display_t *display) {
  if (pdFAIL == xTaskCreate(display_task, "display Task", 4096, (void *)display,
                            1, &display->task))
    esp_system_abort("Failed to create display task!\n");
}

esp_err_t update_config(const display_t *display) {
  esp_err_t error;
  uint8_t write_buf;

  write_buf = 0x20 | display->config.en_oscillator;
  error = i2c_master_transmit(display->i2c_handle, &write_buf, 1, TIMEOUT);
  if (error != ESP_OK)
    return error;

  write_buf =
      0x80 | ((display->config.blink_speed << 1) | display->config.en_display);
  error = i2c_master_transmit(display->i2c_handle, &write_buf, 1, TIMEOUT);
  if (error != ESP_OK)
    return error;

  write_buf = 0xE0 | display->config.brightness;
  error = i2c_master_transmit(display->i2c_handle, &write_buf, 1, TIMEOUT);
  return error;
}

esp_err_t update_animation(display_t *display) {
  display_animation_t *const animation = &display->animation; // Convenience
  uint8_t write_buf_bytes[18] = {0};
  // the display uses 16 bit characters, so this is used to stride correctly
  uint16_t *write_buf = (uint16_t *)&write_buf_bytes[1];
  uint8_t chars = 0;
  uint8_t str_idx = animation->frame;

  // Reset animation (1 frame empty)
  if (animation->frame > strlen(animation->data)) {
    animation->frame = 0;
    str_idx = 0;
  }

  while (chars < 5 && str_idx < animation->frame + 8 &&
         str_idx < strlen(animation->data)) {
    if (chars == 2)
      chars++;

    if (chars == 0 && animation->data[str_idx] == '.') {
      str_idx++;
    }

    write_buf[chars] = char_to_font(animation->data[str_idx]);
    str_idx++;
    if (animation->data[str_idx] == '.') {
      write_buf[chars] |= FONT_CHAR_DECIMAL;
      str_idx++;
    }

    chars++;
  }

  return i2c_master_transmit(display->i2c_handle, ((uint8_t *)write_buf_bytes),
                             17, TIMEOUT);
}
