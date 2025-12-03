#include "rtc.h"

#include <stdint.h>
#include <stm32f103x6.h>

void rtc_config(rtc_config_t config) {
  config.prescalar &= (0xFFFFF); /* Keep 20 LSBs */

  /* Enable RTC */
  RCC->BDCR |= RCC_BDCR_RTCEN;
  RCC->BDCR |= RCC_BDCR_LSEON;
  RCC->BDCR |= (1U << RCC_BDCR_RTCSEL_Pos);

  /* Enable Configuration */
  while (!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;

  /* Prescalar */
  RTC->PRLH |= config.prescalar >> 16;
  RTC->PRLL |= config.prescalar & 0xFFFF;

  /* Alarm */
  RTC->CRH |= RTC_CRH_ALRIE;
  RTC->ALRH |= config.alarm >> 16;
  RTC->ALRL |= config.alarm & 0xFFFF;

  /* Disable Configuration */
  RTC->CRL &= ~(RTC_CRL_CNF);
  while (!(RTC->CRL & RTC_CRL_RTOFF));
}

rtc_time_t rtc_read(void) {
  uint16_t raw = RTC->CNTL;

  return (rtc_time_t){
      .hour = (raw / 3600) % 24,
      .minute = (raw / 60),
      .second = raw % 60,
  };
}
