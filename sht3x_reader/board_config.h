/*
 * board_config.h
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */

#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include "SHT3x.h"

#define	temperature_sensor_init			sht3x_init
#define temperature_read				sht3x_temperature_blocking_read

#endif /* BOARD_CONFIG_H_ */
