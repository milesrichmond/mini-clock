#pragma once

#include <stdint.h>

typedef struct rtc_time_t {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} rtc_time_t;

typedef struct rtc_config_t {
  uint32_t prescalar;
  uint32_t alarm;
} rtc_config_t;

static const rtc_config_t rtc_default_config = {
    .prescalar = (1U << 15) - 1,
    .alarm = 60,
};

void rtc_config(rtc_config_t config);

rtc_time_t rtc_read(void);
