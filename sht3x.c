#include "sht3x.h"
#include "pl_hw_config.h"
#include "pl_config.h"
#include <string.h>

/* SHT3x Commands */
#define SHT3X_CMD_MEASURE_HIGH_REP    0x2400  /* High repeatability measurement */
#define SHT3X_CMD_MEASURE_MEDIUM_REP  0x240B  /* Medium repeatability measurement */
#define SHT3X_CMD_MEASURE_LOW_REP     0x2416  /* Low repeatability measurement */
#define SHT3X_CMD_SOFT_RESET          0x30A2  /* Soft reset */
#define SHT3X_CMD_STATUS              0xF32D  /* Read status register */

/* CRC-8 polynomial for SHT3x: x^8 + x^5 + x^4 + 1 */
#define SHT3X_CRC_POLYNOMIAL 0x31

/**
 * @brief Calculate CRC-8 checksum for SHT3x
 * @param data Pointer to data
 * @param len Length of data
 * @return CRC-8 checksum
 */
static uint8_t sht3x_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;
    uint8_t i, j;
    
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ SHT3X_CRC_POLYNOMIAL;
            } else {
                crc = crc << 1;
            }
        }
    }
    
    return crc;
}

int sht3x_init(void)
{
    /* Initialize I2C */
    if (pl_i2c_init() != 0) {
        return SHT3X_ERR_I2C;
    }
    
    /* Wait for sensor to be ready */
    pl_delay_ms(50);
    
    /* Perform soft reset */
    return sht3x_reset();
}

int sht3x_reset(void)
{
    uint8_t cmd[2];
    
    /* Prepare soft reset command */
    cmd[0] = (SHT3X_CMD_SOFT_RESET >> 8) & 0xFF;
    cmd[1] = SHT3X_CMD_SOFT_RESET & 0xFF;
    
    /* Send reset command */
    if (pl_i2c_write(SHT3X_I2C_ADDRESS, cmd, 2) != 0) {
        return SHT3X_ERR_I2C;
    }
    
    /* Wait for reset to complete */
    pl_delay_ms(50);
    
    return SHT3X_OK;
}

int sht3x_read(float *temperature, float *humidity)
{
    uint8_t cmd[2];
    uint8_t data[6];
    uint16_t temp_raw, hum_raw;
    
    /* Prepare measurement command (high repeatability) */
    cmd[0] = (SHT3X_CMD_MEASURE_HIGH_REP >> 8) & 0xFF;
    cmd[1] = SHT3X_CMD_MEASURE_HIGH_REP & 0xFF;
    
    /* Send measurement command */
    if (pl_i2c_write(SHT3X_I2C_ADDRESS, cmd, 2) != 0) {
        return SHT3X_ERR_I2C;
    }
    
    /* Wait for measurement to complete (high repeatability: ~15ms) */
    pl_delay_ms(20);
    
    /* Read measurement result (6 bytes: temp_msb, temp_lsb, temp_crc, hum_msb, hum_lsb, hum_crc) */
    if (pl_i2c_read(SHT3X_I2C_ADDRESS, data, 6) != 0) {
        return SHT3X_ERR_I2C;
    }
    
    /* Verify CRC for temperature */
    if (sht3x_crc8(data, 2) != data[2]) {
        return SHT3X_ERR_CRC;
    }
    
    /* Verify CRC for humidity */
    if (sht3x_crc8(data + 3, 2) != data[5]) {
        return SHT3X_ERR_CRC;
    }
    
    /* Convert raw values */
    temp_raw = (data[0] << 8) | data[1];
    hum_raw = (data[3] << 8) | data[4];
    
    /* Calculate temperature in Celsius: T = -45 + 175 * (temp_raw / 65535) */
    if (temperature != NULL) {
        *temperature = -45.0f + 175.0f * ((float)temp_raw / 65535.0f);
    }
    
    /* Calculate relative humidity in %: RH = 100 * (hum_raw / 65535) */
    if (humidity != NULL) {
        *humidity = 100.0f * ((float)hum_raw / 65535.0f);
    }
    
    return SHT3X_OK;
}
