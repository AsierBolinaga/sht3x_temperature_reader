/*
 * energy_task.c
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board_config.h"
#include "comm.h"
#include "debug_console.h"
#include "tempreader.h"
#include "board_config.h"

#include "pl_hw_config.h"
#include "pl_socket.h"
#include "pl_thread.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

static char* READ_MODULE_NAME_ASCII	= "$01M";
static char* ENABLE_CHANNELS_ASCII 	= "$015";
static char* SELECT_RANGE_ASCII 	= "$01A00";
static char* READ_VALUES_ASCII 		= "#01";

static char* MODULE_NAME 			= "XXX6015";
static char* DUMMY_ANSWER 			= "XXX";

/*******************************************************************************
 * type definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static comm_thread_config_t* comm_config;

static pl_socket_t			pl_socket_comm;
static pl_socket_config_t* 	socket_comm__config;

static char rx_buf[BUFLEN];

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
bool comm_task_initialize(comm_thread_config_t* _comm_task_config)
{
	bool return_value = false;

	if(NULL != _comm_task_config)
	{
		comm_config = _comm_task_config;

		socket_comm__config = pl_config_get_socket_conf(comm_config->comm_socket_index);
		if(PL_SOCKET_RV_OK == pl_socket_init(&pl_socket_comm, socket_comm__config, NULL))
		{
			pl_socket_set_buff(&pl_socket_comm,  rx_buf);
			comm_config->comm_initialized = true;
			return_value = true;
		}
		else
		{
			print_console("socket error\n");
		}
	}

	return return_value;
}

/*!
 * @brief Entry point of the energy task
 *
 * @param *arg  Pointer to energy task parameters
 */
void comm_task(void* arg)
{
	float 	temperature;
	char 	temp_str[20];
	uint32_t received_length;
	uint32_t received_port;
	
	int  i, slen, recv_len;
	
	if(!comm_config->comm_initialized)
	{
		print_console("ERROR! comm thread has not been initialized!\n");
		while(1);
	}

	if(PL_SOCKET_RV_OK != pl_socket_create(&pl_socket_comm))
	{
		print_console("Socket creation error\n");
		while(1);
	}

	while(1)
	{
		if(PL_SOCKET_RV_OK == pl_socket_receive(&pl_socket_comm, &received_length))
		{
			/* Todo - Commands done to be compatible with ADAM sources. Should be implemented in another way
			when the AICT is ready */
			if(strstr(rx_buf, READ_VALUES_ASCII))
			{
				print_console("Temperature command received!\n");
				temperature = tempreader_get_temperature() / 1000.0f;
				print_console("Measured temperature: %0.3f degreeCelsius.\n", temperature);
				snprintf(temp_str, 10, ">+%0.3f" , temperature);

				if(PL_SOCKET_RV_OK != pl_socket_transfer(&pl_socket_comm, temp_str, sizeof(temp_str),
						pl_socket_get_remote_port(&pl_socket_comm)))
				{
					print_console("UDP transfer error!");
				}
			}
			else if(strstr(rx_buf, READ_MODULE_NAME_ASCII))
			{
				print_console("Module name command received!\n");

				if(PL_SOCKET_RV_OK != pl_socket_transfer(&pl_socket_comm, MODULE_NAME, 7,
						              pl_socket_get_remote_port(&pl_socket_comm)))
				{
					print_console("UDP transfer error!");
				}
			}
			else if(strstr(rx_buf, ENABLE_CHANNELS_ASCII))
			{
				print_console("Enable channels command received!\n");

				if(PL_SOCKET_RV_OK != pl_socket_transfer(&pl_socket_comm, DUMMY_ANSWER, 3,
						                                 pl_socket_get_remote_port(&pl_socket_comm)))
				{
					print_console("UDP transfer error!");
				}
			}
			else if(strstr(rx_buf, SELECT_RANGE_ASCII))
			{
				print_console("Select range command received!\n");

				if(PL_SOCKET_RV_OK != pl_socket_transfer(&pl_socket_comm, DUMMY_ANSWER, 3,
						                                  pl_socket_get_remote_port(&pl_socket_comm)))
				{
					print_console("UDP transfer error!");
				}
			}
			else
			{
				print_console("Unknown command received: %s\n", rx_buf);
			}
		}
	}
}
