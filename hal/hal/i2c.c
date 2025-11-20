#include "i2c.h"

#include <stdint.h>
#include <stm32f103x6.h>

/* --- DEFINITIONS --- */
#define TIMEOUT 1000

#define I2C_REG_SIZE sizeof(I2C_TypeDef)
#define I2C_REG(port) ((I2C_TypeDef*)(I2C1_BASE * (I2C_REG_SIZE * (port))))

/* Simple error handler */
#define ERR(expr) do {uint8_t err; if ((err = (expr))) return err;} while(0)

/* --- BASIC OPERATION PROTOTYPES --- */

uint8_t generate_start(I2C_TypeDef *reg);

void generate_stop(I2C_TypeDef *reg);

void clear_addr_flag(I2C_TypeDef *reg);

uint8_t send_dev_addr(I2C_TypeDef *reg, uint8_t addr);

uint8_t send_byte(I2C_TypeDef *reg, uint8_t byte);

uint8_t read_byte(I2C_TypeDef *reg, uint8_t *data);

/* --- HEADER IMPLEMENTATION --- */

void i2c_init(i2c_port_t port) {
    I2C_TypeDef *i2c_reg = I2C_REG(port);

    RCC->APB1ENR |= (RCC_APB1ENR_I2C1EN << port);

    i2c_reg->CR2 &= ~(I2C_CR2_FREQ);
    i2c_reg->CR2 |= 8U << I2C_CR2_FREQ_Pos; /* 8 MHz */
    i2c_reg->TRISE = 0x9;
    i2c_reg->CCR |= 0x28;
}

void i2c_enable(i2c_port_t port) {
    I2C_TypeDef *i2c_reg = I2C_REG(port);
    i2c_reg->CR1 |= I2C_CR1_PE;
}

void i2c_disable(i2c_port_t port) {
    I2C_TypeDef *i2c_reg = I2C_REG(port);
    i2c_reg->CR1 &= ~(I2C_CR1_PE);
}

uint8_t i2c_read(i2c_device_t dev, uint8_t dev_reg, uint8_t n, uint8_t *data) {
    I2C_TypeDef *i2c_reg = I2C_REG(dev.port);

    while (i2c_reg->SR2 & I2C_SR2_BUSY);
    ERR(generate_start(i2c_reg));
    ERR(send_dev_addr(i2c_reg, dev.address << 1));
    ERR(send_byte(i2c_reg, dev_reg));

    /* Control over the bus is never released; we dont have to check SR2_BUSY */
    ERR(generate_start(i2c_reg));
    ERR(send_dev_addr(i2c_reg, (dev.address << 1) | 1));
    clear_addr_flag(i2c_reg);

    i2c_reg->CR1 |= I2C_CR1_ACK; /* enable ACK */
    while (n > 0) {
        if (n == 1) {
            i2c_reg->CR1 &= ~(I2C_CR1_ACK); /* ACK final byte */
            generate_stop(i2c_reg); /* Stop transmission */
        }

        ERR(read_byte(i2c_reg, data++));
        n--;
    }
    return 0; /* success */
}

uint8_t i2c_write(i2c_device_t dev, uint8_t dev_reg, uint8_t n, uint8_t *data) {
    I2C_TypeDef *i2c_reg = I2C_REG(dev.port);

    while (i2c_reg->SR2 & I2C_SR2_BUSY);
    ERR(generate_start(i2c_reg));
    ERR(send_dev_addr(i2c_reg, dev.address << 1));
    clear_addr_flag(i2c_reg);

    ERR(send_byte(i2c_reg, dev_reg));
    while (n > 0) {
        ERR(send_byte(i2c_reg, *data++));
        n--;
    }
    generate_stop(i2c_reg);
    return 0; /* success */
}

uint8_t i2c_send_command(i2c_device_t dev, uint8_t cmd) {
    I2C_TypeDef *i2c_reg = I2C_REG(dev.port);

    while(i2c_reg->SR2 & I2C_SR2_BUSY);
    ERR(generate_start(i2c_reg));
    ERR(send_dev_addr(i2c_reg, dev.address << 1));
    clear_addr_flag(i2c_reg);

    ERR(send_byte(i2c_reg, cmd));

    generate_stop(i2c_reg);
    return 0; /* success */
}

/* --- BASIC OPERATION IMPLEMENTATION --- */

uint8_t generate_start(I2C_TypeDef *reg) {
    reg->CR1 |= I2C_CR1_START;

    uint16_t counter = TIMEOUT;
    while (!(reg->SR1 & I2C_SR1_SB) && --counter);

    return (counter) ? 0 : 1; /* success : timeout */
}

void generate_stop(I2C_TypeDef *reg) {
    reg->CR1 |= I2C_CR1_STOP;
}

void clear_addr_flag(I2C_TypeDef *reg) {
    volatile uint16_t tmp = reg->SR1;
    tmp = reg->SR2;
    (void)tmp;
}

uint8_t send_dev_addr(I2C_TypeDef *reg, uint8_t addr) {
    reg->DR = addr;

    uint16_t counter = TIMEOUT;
    while (!(reg->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) && --counter);

    if (reg->SR1 & I2C_SR1_AF)
        return 2; /* nack */

    if (!counter)
        return 1; /* timeout */

    clear_addr_flag(reg);
    return 0;
}

uint8_t send_byte(I2C_TypeDef *reg, uint8_t byte) {
    reg->DR = byte;

    uint16_t counter = TIMEOUT;
    while (!(reg->SR1 & I2C_SR1_TXE) && --counter);

    if (!counter)
        return 1; /* timeout */

    return 0;
}

uint8_t read_byte(I2C_TypeDef *reg, uint8_t *data) {
    uint16_t counter = TIMEOUT;
    while (!(reg->SR1 & I2C_SR1_RXNE) && --counter);

    if (!counter)
        return 1; /* timeout */

    *data = reg->DR;
    return 0;
}
