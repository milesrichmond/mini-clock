#pragma once

#include <hal/i2c.h>
#include <stdint.h>

/*
 * The sensor is also able to activate a heater with various wattages, but we
 * probably do not need that.
*/
typedef enum sht45_codes_t {
    SHT45_TRH_HIGH = 0xFD,
    SHT45_TRH_MED = 0xF6,
    SHT45_TRH_LOW = 0xE0,

    SHT45_SERIAL_NUM = 0x89,
    SHT45_RESET = 0x94,
} sht45_codes_t;

typedef struct sht45_data_t {
    float temp;
    float rh;
} sht45_data_t;

void sht45_read_data(i2c_device_t dev, sht45_codes_t type, sht45_data_t *data);
