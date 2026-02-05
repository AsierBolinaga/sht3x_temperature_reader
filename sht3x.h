#ifndef SHT3X_H
#define SHT3X_H

#include <stdint.h>

/* SHT3x return codes */
#define SHT3X_OK           0
#define SHT3X_ERR_I2C     -1
#define SHT3X_ERR_CRC     -2
#define SHT3X_ERR_TIMEOUT -3

/**
 * @brief Initialize SHT3x sensor
 * @return SHT3X_OK on success, negative on error
 */
int sht3x_init(void);

/**
 * @brief Read temperature and humidity from SHT3x sensor
 * @param temperature Pointer to store temperature in Celsius
 * @param humidity Pointer to store relative humidity in %
 * @return SHT3X_OK on success, negative on error
 */
int sht3x_read(float *temperature, float *humidity);

/**
 * @brief Soft reset SHT3x sensor
 * @return SHT3X_OK on success, negative on error
 */
int sht3x_reset(void);

#endif /* SHT3X_H */
