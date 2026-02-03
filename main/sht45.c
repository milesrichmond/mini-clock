#include "sht45.h"

#include <string.h>

#include "i2c.h"

typedef enum sht45_command_t {
  SHT45_TRH_HIGH = 0xFD,
  SHT45_TRH_MED = 0xF6,
  SHT45_TRH_LOW = 0xE0,

  SHT45_SERIAL_NUM = 0x89,
  SHT45_RESET = 0x94,
} sht45_command_t;

void sht45_task(void *parameters) {
  sht45_t *const dev = (sht45_t *)parameters;
  TickType_t lastWakeTime = xTaskGetTickCount();
  const uint8_t command = SHT45_TRH_HIGH;

  uint8_t read_buffer[6];
  memset(read_buffer, 0, sizeof(read_buffer));
  uint16_t staging_buffer; /* Used to reformat read_buffer */

  for (;;) {
    /* Read raw data */
    ESP_ERROR_CHECK(i2c_master_transmit(dev->i2c_handle, &command,
                                        sizeof(command), I2C_STD_TIMEOUT));
    vTaskDelay(pdMS_TO_TICKS(10)); /* Device requires > 8.3 ms wait */
    ESP_ERROR_CHECK(i2c_master_receive(dev->i2c_handle, read_buffer,
                                       sizeof(read_buffer), I2C_STD_TIMEOUT));

    /* Convert to human format */
    staging_buffer = ((uint16_t)read_buffer[0] << 8) | (uint16_t)read_buffer[1];
    if (dev->config.fahrenheit) {
      dev->temp = -49 + (315 * (staging_buffer / 65535.0f));
    } else {
      dev->temp = -45 + (175 * (staging_buffer / 65535.0f));
    }

    staging_buffer = ((uint16_t)read_buffer[3] << 8) | (uint16_t)read_buffer[4];
    dev->rh = -6 + (125 * (staging_buffer / 65535.0f));

    /* Sleep for a while */
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(dev->config.update_time_ms));
  }

  vTaskDelete(NULL);
}

void sht45_init(uint8_t bus, sht45_t *const sht45) {
  static const i2c_device_config_t dev_config = {
      .device_address = 0x44,
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .scl_speed_hz = 100000,
      .flags.disable_ack_check = true,
  };

  sht45->i2c_handle = init_i2c_dev(bus, &dev_config);

  if (pdFAIL == xTaskCreate(sht45_task, "SHT45 Task", 4096, (void *)sht45, 1,
                            &sht45->task))
    esp_system_abort("Failed to create SHT45 task!\n");
}
