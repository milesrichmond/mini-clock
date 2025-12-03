#include "display.h"

#include "i2c.h"

/* --- IMPLEMENTATION --- */

extern void display_enable(i2c_device_t dev);

extern void display_disable(i2c_device_t dev);

void display_config(i2c_device_t dev, display_config_t config, uint8_t duty) {
  config &= 0x08; /* mask for 3 LSBs */
  duty &= 0x0F;   /* mask for 4 LSBs */
  i2c_send_command(dev, DISPLAY_DATA_ADDR | (uint8_t)config);
  i2c_send_command(dev, DISPLAY_DIMMING | duty);
}

void display_write(i2c_device_t dev, uint8_t bytes, uint8_t* data) {
  /* Unfortunate mismatch between display type and the display's chip. */
  uint8_t display_data[bytes * 2];
  for (uint8_t i = 0; i < bytes; i++) {
    display_data[2 * i] = *data++;
    display_data[(2 * i) + 1] = 0;
  }

  i2c_write(dev, DISPLAY_DATA_ADDR, bytes * 2, display_data);
}
