/*
 * pl_config.c
 *
 *  Created on: 1 feb. 2022
 *      Author: Asier Bolinaga
 */
#include "pl_hw_config.h"
#include "pl_config.h"
#include "pl_types.h"
#ifdef PL_IMX_RT10XX
#include "board.h"
//#include "fsl_enet_mdio.h"
#include "fsl_phydp83825.h"
#endif

//#define TEMP_I2C_MASTER 	Driver_I2C1
#define TEMP_I2C_MASTER 	2

#define I2C_BAUDRATE (100000) /* 100K */

static pl_i2c_config_t  i2c_sht3x_config =
{
#if defined(PL_BEAGLEBONE) || defined(PL_PC)
	TEMP_I2C_MASTER,
#endif
	PL_I2C_MODE_MASTER,
	1000
};

static pl_i2c_config_t* 	i2c_configs[PL_I2C_MAXVALUE] =
{
	&i2c_sht3x_config
};

pl_i2c_config_t*    pl_config_get_i2c_conf(i2c_t _i2c)
{
	return i2c_configs[_i2c];
}

/*************************************************************************/
/*							PHY CONFIGURATION       					 */
/*************************************************************************/
#define TEENSY_PHY_ADDR	0

extern  phy_dp83825_resource_t g_phy_resource;
static  phy_handle_t phyHandle;

static  pl_phy_config_t temp_sensor_phy=
{
		false,
		{192, 168, 2, 105},
		{255, 255, 255, 0},
		{192, 168, 2, 72},
		{0x02, 0x12, 0x13, 0x10, 0x15, 0x11},
		&phyHandle,
		TEENSY_PHY_ADDR,
		&phydp83825_ops,
		&g_phy_resource,
		kCLOCK_IpgClk,
};

/*************************************************************************/
/*						SOCKET CONFIGURATION       						 */
/*************************************************************************/

static pl_socket_config_t comm_socket_config =
{
		&temp_sensor_phy,
		PL_SOCKET_PROTOCOL_UDP,
		PL_SOCKET_MODE_SERVER,
		{192, 168, 2, 1},
		8888,
		NULL
};

static pl_socket_config_t* 	socket_configs[PL_SOCKET_MAXVALUE] =
{
	&comm_socket_config
};

pl_socket_config_t* pl_config_get_socket_conf(socket_t _socket)
{
	return socket_configs[_socket];
}

