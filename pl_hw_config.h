#ifndef PL_HW_CONFIG_H
#define PL_HW_CONFIG_H

#include "pl_config.h"
#include <stdint.h>

/* Hardware abstraction layer function declarations */

/**
 * @brief Initialize the hardware platform
 * @return 0 on success, negative on error
 */
int pl_hw_init(void);

/**
 * @brief Initialize I2C interface
 * @return 0 on success, negative on error
 */
int pl_i2c_init(void);

/**
 * @brief Write data to I2C device
 * @param addr I2C device address
 * @param data Pointer to data buffer
 * @param len Length of data
 * @return 0 on success, negative on error
 */
int pl_i2c_write(uint8_t addr, const uint8_t *data, uint16_t len);

/**
 * @brief Read data from I2C device
 * @param addr I2C device address
 * @param data Pointer to receive buffer
 * @param len Length of data to read
 * @return 0 on success, negative on error
 */
int pl_i2c_read(uint8_t addr, uint8_t *data, uint16_t len);

/**
 * @brief Initialize UDP socket
 * @param ip_addr Destination IP address
 * @param port Destination port
 * @return 0 on success, negative on error
 */
int pl_udp_init(const char *ip_addr, uint16_t port);

/**
 * @brief Send data via UDP
 * @param data Pointer to data buffer
 * @param len Length of data
 * @return 0 on success, negative on error
 */
int pl_udp_send(const uint8_t *data, uint16_t len);

/**
 * @brief Close UDP socket
 */
void pl_udp_close(void);

/**
 * @brief Close I2C interface
 */
void pl_i2c_close(void);

/**
 * @brief Cleanup all hardware resources
 */
void pl_hw_cleanup(void);

/**
 * @brief Delay in milliseconds
 * @param ms Milliseconds to delay
 */
void pl_delay_ms(uint32_t ms);

/**
 * @brief Get current tick count in milliseconds
 * @return Current tick count
 */
uint32_t pl_get_tick_ms(void);

#endif /* PL_HW_CONFIG_H */
