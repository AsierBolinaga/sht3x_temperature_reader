/*
 * pl_config.c
 *
 *  Created on: 1 feb. 2022
 *      Author: Asier Bolinaga
 */
#include "pl_hw_config.h"
#include "pl_types.h"
#ifdef PL_IMX_RT10XX
#include "board.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
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

#if defined(PL_OS_FREE_RTOS)
static mdio_handle_t mdioHandle = {.ops = &enet_ops};
static phy_handle_t phyHandle   = {
		.phyAddr = BOARD_ENET0_PHY_ADDRESS,
		.mdioHandle = &mdioHandle,
		.ops = &phyksz8081_ops};

static pl_socket_port_config_t socket_port_config =
{
		false,
		{192, 168, 56, 1},
		{255, 255, 255, 0},
		{192, 168, 10, 2},
		{0x02, 0x12, 0x13, 0x10, 0x15, 0x11},
		&phyHandle,
		kCLOCK_IpgClk
};
#endif

static pl_socket_config_t comm_socket_config =
{
#if defined(PL_OS_FREE_RTOS)
	&socket_port_config,
#endif
	PL_SOCKET_PROTOCOL_UDP,
	PL_SOCKET_MODE_SERVER,
#if defined(PL_OS_FREE_RTOS)
	{192, 168, 56, 7}, /* Any IP */
#elif PL_LINUX
	PL_SOCKET_ANY_IP_ADDR,
#endif
	8888
};

static pl_socket_config_t*	socket_configs[PL_SOCKET_MAXVALUE] =
{
	&comm_socket_config
};

pl_socket_config_t* pl_config_get_socket_conf(socket_t _socket)
{
	return socket_configs[_socket];
}

