/*
 * ade9000.c
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */
#include "SHT3x.h"
#include "debug_console.h"
#include "pl_hw_config.h"
#include "pl_i2c.h"
#include "pl_thread.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* all measurement commands return T (CRC) RH (CRC) */
#if USE_SENSIRION_CLOCK_STRETCHING
#define SHT3X_CMD_MEASURE_HPM 0x2C06
#define SHT3X_CMD_MEASURE_MPM 0x2C0D
#define SHT3X_CMD_MEASURE_LPM 0x2C10
#else /* USE_SENSIRION_CLOCK_STRETCHING */
#define SHT3X_CMD_MEASURE_HPM 0x2400
#define SHT3X_CMD_MEASURE_MPM 0x240B
#define SHT3X_CMD_MEASURE_LPM 0x2416
#endif /* USE_SENSIRION_CLOCK_STRETCHING */

#define SHT3X_HUMIDITY_LIMIT_MSK 0xFE00U
#define SHT3X_TEMPERATURE_LIMIT_MSK 0x01FFU

static const uint16_t SHT3X_CMD_READ_STATUS_REG = 0xF32D;
static const uint16_t SHT3X_CMD_CLR_STATUS_REG = 0x3041;
static const uint16_t SHT3X_CMD_READ_SERIAL_ID = 0x3780;
static const uint16_t SHT3X_CMD_DURATION_USEC = 1000;

static const uint16_t SHT3X_CMD_MEASURE 		    = 0x2400;
static const uint16_t SHT3X_CMD_CMEASURE_HUMINITY 	= 0x05;
/* read commands for the alert settings */
static const uint16_t SHT3X_CMD_READ_HIALRT_LIM_SET = 0xE11F;
static const uint16_t SHT3X_CMD_READ_HIALRT_LIM_CLR = 0xE114;
static const uint16_t SHT3X_CMD_READ_LOALRT_LIM_CLR = 0xE109;
static const uint16_t SHT3X_CMD_READ_LOALRT_LIM_SET = 0xE102;
/* write commands for the alert settings */
static const uint16_t SHT3X_CMD_WRITE_HIALRT_LIM_SET = 0x611D;
static const uint16_t SHT3X_CMD_WRITE_HIALRT_LIM_CLR = 0x6116;
static const uint16_t SHT3X_CMD_WRITE_LOALRT_LIM_CLR = 0x610B;
static const uint16_t SHT3X_CMD_WRITE_LOALRT_LIM_SET = 0x6100;

/**
 * @brief SHT3x I2C 7-bit SHT3X_I2C_ADDR_DFLTess option
 */
typedef enum _sht3x_i2c_SHT3X_I2C_ADDR_DFLT {
    SHT3X_I2C_ADDR_DFLT = 0x44,
    SHT3X_I2C_ADDR_ALT = 0x45
} sht3x_i2c_SHT3X_I2C_ADDR_DFLT_t;

/*******************************************************************************
 * type definitions
 ******************************************************************************/

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint16_t sht3x_cmd_measure = SHT3X_CMD_MEASURE_HPM;

static pl_i2c_t			temp_i2c;
static pl_i2c_config_t* temp_i2c_config;

static uint8_t	tx_buff[32];

/*******************************************************************************
 * Code
 ******************************************************************************/
bool sht3x_init(tempreader_config_t* temp_config)
{
	bool return_value = false;

	temp_i2c_config = pl_config_get_i2c_conf(temp_config->i2c_conf_index);
    if(PL_I2C_RV_OK == pl_i2c_init(&temp_i2c, temp_i2c_config))
	{
		return_value = true;
	}

	return return_value;
}

bool sht3x_temperature_blocking_read(int32_t* temperature)
{
    bool return_value = false;
	uint8_t	command[2];
	uint8_t measures[6];

	command[0] = sht3x_cmd_measure >> 8;
	command[1] = sht3x_cmd_measure;

    if(PL_I2C_RV_OK == pl_i2c_transfer(&temp_i2c, SHT3X_I2C_ADDR_DFLT, command, 2))
	{
        pl_thread_sleep(20);
    	if(PL_I2C_RV_OK == pl_i2c_receive(&temp_i2c, SHT3X_I2C_ADDR_DFLT, measures, 6))
    	{
    		uint16_t temp = (measures[0] << 8) + measures[1];
    		tick_to_temperature(temp, temperature);
    		return_value = true; 
    	}
	}

    return return_value;
}  

void tick_to_temperature(uint16_t tick, int32_t* temperature)
{
    *temperature = ((21875 * (int32_t)tick) >> 13) - 45000;
}

void tick_to_humidity(uint16_t tick, int32_t* humidity) {
    *humidity = ((12500 * (int32_t)tick) >> 13);
}

void temperature_to_tick(int32_t temperature, uint16_t* tick)
{
    *tick = (uint16_t)((temperature * 12271 + 552195000) >> 15);
}

void humidity_to_tick(int32_t humidity, uint16_t* tick)
{
    *tick = (uint16_t)((humidity * 21474) >> 15);
}
