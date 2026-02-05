/*
 * ade9000.h
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */

#ifndef SHT3X_H
#define SHT3X_H

#include "board_types.h"
#include "pl_types.h"


#define STATUS_OK 0
#define STATUS_ERR_BAD_DATA (-1)
#define STATUS_CRC_FAIL (-2)
#define STATUS_UNKNOWN_DEVICE (-3)
#define STATUS_ERR_INVALID_PARAMS (-4)
#define SHT3X_MEASUREMENT_DURATION_USEC 15000

/* status word macros */
#define SHT3X_IS_ALRT_PENDING(status) (((status)&0x8000U) != 0U)
#define SHT3X_IS_ALRT_RH_TRACK(status) (((status)&0x0800) != 0U)
#define SHT3X_IS_ALRT_T_TRACK(status) (((status)&0x0400U) != 0U)
#define SHT3X_IS_SYSTEM_RST_DETECT(status) (((status)&0x0010U) != 0U)
#define SHT3X_IS_LAST_CRC_FAIL(status) (((status)&0x0001U) != 0U)

/**
 * @brief SHT3x measurment mode options (Low, Medium and High rerefresh rates)
 */
typedef enum _sht3x_measurement_mode {
    SHT3X_MEAS_MODE_LPM, /*low power mode*/
    SHT3X_MEAS_MODE_MPM, /*medium power mode*/
    SHT3X_MEAS_MODE_HPM  /*high power mode*/
} sht3x_measurement_mode_t;

/**
 * @brief SHT3x Alert Thresholds
 */
typedef enum _sht3x_alert_thd {
    SHT3X_HIALRT_SET,
    SHT3X_HIALRT_CLR,
    SHT3X_LOALRT_CLR,
    SHT3X_LOALRT_SET,
} sht3x_alert_thd_t;


bool sht3x_init(tempreader_config_t* temp_config);

/**
 * @brief Starts a measurement and then reads out the results. This function
 * blocks while the measurement is in progress. The duration of the measurement
 * depends on the sensor in use, please consult the datasheet. Temperature is
 * returned in [degree Celsius], multiplied by 1000, and relative humidity in
 * [percent relative humidity], multiplied by 1000.
 *
 * @param[in]  addr the sensor address
 * @param[out] temperature   the address for the result of the temperature
 * measurement
 * @param[out] humidity      the address for the result of the relative humidity
 * measurement
 *
 * @return              0 if the command was successful, else an error code.
 */
//int16_t sht3x_measure_blocking_read(int32_t* temperature, int32_t* humidity);
bool sht3x_temperature_blocking_read(int32_t* temperature);

/**
 * @brief converts temperature from ADC ticks
 *
 * @param tick sensor ADC ticks
 * @param temperature temperature value in T°C*1000
 */
void tick_to_temperature(uint16_t tick, int32_t* temperature);

/**
 * @brief converts humidity from ADC ticks
 *
 * @param tick sensor ADC ticks
 * @param humidity humidity value in %*1000
 */
void tick_to_humidity(uint16_t tick, int32_t* humidity);

/**
 * @brief converts temperature to ADC ticks
 *
 * @param temperature temperature value in T°C*1000
 * @param tick sensor ADC ticks
 */
void temperature_to_tick(int32_t temperature, uint16_t* tick);

/**
 * @brief converts humidity to ADC ticks
 *
 * @param humidity humidity value in %*1000
 * @param tick sensor ADC ticks
 */
void humidity_to_tick(int32_t humidity, uint16_t* tick);

#endif /* SHT3X_H */
