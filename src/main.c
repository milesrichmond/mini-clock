#include <stdint.h>
#include <stm32f103x6.h>

#include "dev/display.h"
#include "dev/sht45.h"
#include "hal/i2c.h"
#include "hal/rtc.h"

/* --- PROTOTYPES --- */

static inline uint8_t bkp_has_init(void) { return (BKP_DR1_D) ? 1 : 0; }

static inline void standby(void) {
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  PWR->CR |= PWR_CR_PDDS;
  PWR->CR |= PWR_CR_CWUF;
}

void process_rtc(rtc_time_t* time);

void process_sht45(sht45_data_t* data);

/* --- GLOBALS --- */

static const uint8_t display_decoder[16] = {
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x67, /* 9 */
    0x77, /* A */
    0x7C, /* B */
    0x39, /* C */
    0x5E, /* D */
    0x79, /* E */
    0x71  /* F */
};

#define DISPLAY_DATA_LENGTH (5)
static uint8_t rtc_display_data[DISPLAY_DATA_LENGTH];
static uint8_t sht45_display_data[DISPLAY_DATA_LENGTH];

static const i2c_device_t rtc_display = {.port = I2C_PORT_1, .address = 0x71};
static const i2c_device_t sht45_display = {.port = I2C_PORT_1, .address = 0x72};

/* --- MAIN --- */

/* Wake on rtc alarm interrupt */
/* As there are more functions to the clock, there will be external interrupts,
 * that will wake the mcu, but their behaviors must be handled separately. */
int main(void) {
  bkp_enable();
  i2c_init(I2C_PORT_1);
  i2c_enable(I2C_PORT_1);

  /* First boot init */
  if (!bkp_has_init()) {
    rtc_config_t(rtc_default_config);

    display_enable(rtc_display);
    display_config(rtc_display, DISPLAY_ON, 0xF);

    display_enable(sht45_display);
    display_config(sht45_display, DISPLAY_ON, 0xF);
  }

/* Produce a running loop if standby is not enabled */
#ifdef NO_STANDBY
  while (1) {
#endif

    rtc_time_t time;
    process_rtc(&time);

    /* Temp/Humidity */
    if (time.minute % 5 == 0) {
      sht45_data_t sensor_data;
      process_sht45(&sensor_data);
    }

    /* Write to display */
    display_write(rtc_display, DISPLAY_DATA_LENGTH, rtc_display_data);
    if (time.minute % 5 == 0) {
      display_write(sht45_display, DISPLAY_DATA_LENGTH, sht45_display_data);
    }

/* Close running loop */
#ifdef NO_STANDBY
  }
#else
  standby();
#endif

  return 0;
}

/* Simply clear the flag, the system will reset */
void RTC_ALARM_IRQn(void) { RTC->CRL &= ~(RTC_CRL_ALRF); }

void process_rtc(rtc_time_t* time) {
  *time = rtc_read();
  rtc_display_data[0] = display_decoder[time->hour / 10];
  rtc_display_data[1] = display_decoder[time->hour % 10];
  rtc_display_data[2] = 0xFF;
  rtc_display_data[3] = display_decoder[time->minute / 10];
  rtc_display_data[4] = display_decoder[time->minute % 10];
}

void process_sht45(sht45_data_t* data) {
  static const i2c_device_t sht45 = {.port = I2C_PORT_1, .address = 0x44};

  sht45_read_data(sht45, SHT45_TRH_HIGH, data);

  sht45_display_data[0] = display_decoder[((int)data->rh) / 10];
  sht45_display_data[1] = display_decoder[((int)data->rh) % 10];
  sht45_display_data[2] = 0xFF;
  sht45_display_data[3] = display_decoder[((int)data->temp) / 10];
  sht45_display_data[4] = display_decoder[((int)data->temp) % 10];
}
