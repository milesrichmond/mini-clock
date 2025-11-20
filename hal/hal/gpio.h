#pragma once

#include <stdint.h>

typedef enum gpio_port_t {
    GPIO_PORT_A = 0x0,
    GPIO_PORT_B = 0x1,
    GPIO_PORT_C = 0x2,
    GPIO_PORT_D = 0x3,
    GPIO_PORT_E = 0x4,
    GPIO_PORT_F = 0x5,
    GPIO_PORT_G = 0x6,
} gpio_port_t;

typedef enum gpio_config_t {
    GPIO_CNF_ANALOG = 0x0,
    GPIO_CNF_IN_FLOATING = 0x4,
    GPIO_CNF_IN_PULL = 0x8,
    GPIO_CNF_OUT_PUSH_PULL = 0x0,
    GPIO_CNF_OUT_OPEN_DRAIN = 0x4,
    GPIO_CNF_ALT_OUT_PUSH_PULL = 0x8,
    GPIO_CNF_ALT_OUT_OPEN_DRAIN = 0xC,

    GPIO_MODE_INPUT = 0x0,
    GPIO_MODE_OUTPUT_10MHZ = 0x1,
    GPIO_MODE_OUTPUT_2MHZ = 0x2,
    GPIO_MODE_OUTPUT_50MHZ = 0x3,
} gpio_config_t;

void gpio_port_enable(gpio_port_t port);

void gpio_port_disable(gpio_port_t port);

void gpio_configure(gpio_port_t port, uint8_t pin, gpio_config_t conf);

void gpio_set(gpio_port_t port, uint16_t bsrr_mask);

void gpio_reset(gpio_port_t port, uint16_t bsrr_mask);

void gpio_read(gpio_port_t port, uint16_t idr_mask, uint16_t *data);
