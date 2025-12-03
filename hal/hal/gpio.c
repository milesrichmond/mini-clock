#include "gpio.h"

#include <stdint.h>
#include <stm32f103x6.h>

#define GPIO_REG_SIZE sizeof(GPIO_TypeDef)
/* Potentially cursed, but its pretty sleek */
#define GPIO_REG(port) ((GPIO_TypeDef*)(GPIOA_BASE + (GPIO_REG_SIZE * port)))

void gpio_port_enable(gpio_port_t port) {
  RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN << port);
}

void gpio_port_disable(gpio_port_t port) {
  RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN << port);
}

void gpio_configure(gpio_port_t port, uint8_t pin, gpio_config_t conf) {
  GPIO_TypeDef* gpio_reg = GPIO_REG(port);
  gpio_reg->CRL &= ~(0xF << pin);
  gpio_reg->CRL |= ((conf & 0xF) << pin);
}

void gpio_set(gpio_port_t port, uint16_t bsrr_mask) {
  GPIO_TypeDef* gpio_reg = GPIO_REG(port);
  gpio_reg->BSRR &= ~(bsrr_mask << 16);
  gpio_reg->BSRR |= (bsrr_mask << 16);
}

void gpio_reset(gpio_port_t port, uint16_t bsrr_mask) {
  GPIO_TypeDef* gpio_reg = GPIO_REG(port);
  gpio_reg->BSRR &= ~(bsrr_mask);
  gpio_reg->BSRR |= (bsrr_mask);
}

void gpio_read(gpio_port_t port, uint16_t idr_mask, uint16_t* data) {
  GPIO_TypeDef* gpio_reg = GPIO_REG(port);
  *data = gpio_reg->IDR & idr_mask;
}
