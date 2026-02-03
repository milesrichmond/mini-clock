#include "display.h"

#include <string.h>

#include <esp_system.h>

#include "i2c.h"
#include "util.h"

typedef struct {
  bool en_colon : 1;

  /* an extra 4 characters for '.' */
  char chars[8];
} display_memory_t;

void convert_to_raw(const display_memory_t *data, uint16_t raw[5],
                    uint8_t *frame);

void display_task(void *parameters) {
  disp_t *const display = (disp_t *)parameters;
  TickType_t lastWakeTime = xTaskGetTickCount();
  uint8_t animation_frame = 0;

  display_memory_t disp_mem;
  memset(&disp_mem, 0, sizeof(display_memory_t));
  uint8_t write_buffer[17];
  memset(write_buffer, 0, sizeof(write_buffer));

  /* Running Loop */
  for (;;) {
    /* Update config */
    if (display->update_config) {
      display->update_config = false;

      write_buffer[0] = 0x20 | display->config.en_oscillator;
      ESP_ERROR_CHECK(i2c_master_transmit(display->i2c_device, write_buffer, 1,
                                          I2C_STD_TIMEOUT));

      write_buffer[0] = 0x80 | ((display->config.blink_speed << 1) |
                                display->config.en_display);
      ESP_ERROR_CHECK(i2c_master_transmit(display->i2c_device, write_buffer, 1,
                                          I2C_STD_TIMEOUT));

      write_buffer[0] = 0xE0 | display->config.brightness;
      ESP_ERROR_CHECK(i2c_master_transmit(display->i2c_device, write_buffer, 1,
                                          I2C_STD_TIMEOUT));
    }

    if (display->reset_animation ||
        animation_frame > display->animation.length - 4) {
      animation_frame = 0;
      display->reset_animation = false;
    }
    memcpy(disp_mem.chars, display->animation.str + animation_frame++, 8);

    /* Animate/Display Characters */
    write_buffer[0] = 0x00;
    convert_to_raw(&disp_mem, (uint16_t *)&write_buffer[1], &animation_frame);
    ESP_ERROR_CHECK(i2c_master_transmit(display->i2c_device, write_buffer,
                                        sizeof(write_buffer), I2C_STD_TIMEOUT));

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(display->update_time_ms));
  }

  vTaskDelete(NULL);
}

void disp_init(disp_t *const display) {
  if (pdFAIL == xTaskCreate(display_task, "Display Task", 4096, (void *)display,
                            1, &display->task))
    esp_system_abort("Failed to create display task!\n");
}

void convert_to_raw(const display_memory_t *data, uint16_t raw[5],
                    uint8_t *frame) {
  uint8_t index = 0, chars = 0;

  while (chars < 5 && index < 8) {
    if (chars == 2)
      chars++;

    if (chars == 0 && data->chars[index] == '.') {
      index++;
      (*frame)++;
    }

    raw[chars] = char_to_font(data->chars[index]);
    index++;
    if (data->chars[index] == '.') {
      raw[chars] |= char_to_font('.');
      index++;
    }

    chars++;
  }

  /*
  raw[0] = char_to_font(data->chars[0]) | data->en_decimal_0;
  raw[1] = char_to_font(data->chars[1]) | data->en_decimal_1;
  raw[2] = (data->en_colon) ? 0xFFFF : 0x0000;
  raw[3] = char_to_font(data->chars[2]) | data->en_decimal_2;
  raw[4] = char_to_font(data->chars[3]) | data->en_decimal_3;
  */
}
