/*
 * pl_config.h
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */

#ifndef PL_HW_CONFIG_H_
#define PL_HW_CONFIG_H_

#include "pl_i2c.h"
#include "pl_socket.h"

typedef enum i2c
{
    PL_I2C_TEMP = 0U,
	PL_I2C_MAXVALUE
} i2c_t;

typedef enum socket
{
	PL_SOCKET_COMM = 0U,
	PL_SOCKET_MAXVALUE
} socket_t;


pl_i2c_config_t*    pl_config_get_i2c_conf(i2c_t _i2c);

pl_socket_config_t* pl_config_get_socket_conf(socket_t _socket);


#endif /* PL_HW_CONFIG_H_ */
