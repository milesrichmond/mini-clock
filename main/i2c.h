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

#pragma once

#include <stdint.h>

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_err.h>

#define I2C_STD_TIMEOUT (1000)

/*
 *
 * Initializes the i2c port that this project uses.
 * GPIO22 as SDA, GPIO20 as SCL.
 *
 */
i2c_master_bus_handle_t init_i2c();

i2c_master_dev_handle_t init_i2c_dev(uint8_t bus,
                                     const i2c_device_config_t *dev);
