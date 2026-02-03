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

#include "i2c.h"

#include <freertos/FreeRTOS.h>

i2c_master_bus_handle_t init_i2c() {
  const i2c_master_bus_config_t master_config = {
      .i2c_port = 0,
      .sda_io_num = 22,
      .scl_io_num = 20,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags.enable_internal_pullup = true,
  };
  i2c_master_bus_handle_t handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&master_config, &handle));
  return handle;
}

i2c_master_dev_handle_t init_i2c_dev(uint8_t bus,
                                     const i2c_device_config_t *dev) {
  i2c_master_bus_handle_t bus_handle;
  i2c_master_dev_handle_t dev_handle;

  ESP_ERROR_CHECK(i2c_master_get_bus_handle(bus, &bus_handle));
  ESP_ERROR_CHECK(
      i2c_master_probe(bus_handle, dev->device_address, I2C_STD_TIMEOUT));
  ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, dev, &dev_handle));

  return dev_handle;
}
