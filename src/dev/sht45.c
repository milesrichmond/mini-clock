#include "sht45.h"

typedef struct sht45_data_raw_t {
  uint16_t temp;
  uint16_t rh;
} sht45_data_raw_t;

/* ADJUST DELAY FOR HEATER, OTHERWISE NACK WILL OCCUR! */
void sht45_read_data(i2c_device_t dev, sht45_codes_t type, sht45_data_t* data) {
  sht45_data_raw_t raw;
  uint8_t buf[6];
  i2c_read(dev, type, 6, buf);
  raw.temp = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
  raw.rh = ((uint16_t)buf[3] << 8) | (uint16_t)buf[4];

  data->rh = -6 + 125 * (raw.rh / 65535.0f);
  data->temp = -45 + 175 * (raw.temp / 65535.0f);
}
