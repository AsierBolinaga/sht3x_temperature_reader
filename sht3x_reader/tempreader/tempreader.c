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
#include "debug_console.h"
#include "tempreader.h"
#include "pl_thread.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * type definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static tempreader_thread_config_t* tempereader_config;

static int32_t temperature;

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
bool tempreader_task_initialize(tempreader_thread_config_t* _tempreader_task_config)
{
	bool return_value = false;

	if(NULL != _tempreader_task_config)
	{
		tempereader_config = _tempreader_task_config;
		if(temperature_sensor_init(tempereader_config->temp_module_config))
		{
			tempereader_config->tempreader_initialized = true;
			return_value = true;
		}
	}

	return return_value;
}

/*!
 * @brief Entry point of the energy task
 *
 * @param *arg  Pointer to energy task parameters
 */
void tempreader_task(void* arg)
{

	print_console("Temp sensor reading started:\n");
	if(!tempereader_config->tempreader_initialized)
	{
		print_console("ERROR! Temperature thread has not been initialized!\n");
		while(1);
	}

	print_console("Temp sensor reading started:\n");
	while(1)
	{
		/* Measure temperature and relative humidity and store into variables
		 * temperature, humidity (each output multiplied by 1000).
		 */
		if (!temperature_read(&temperature))
		{
			print_console("error reading measurement\n");
		}
//		print_console("%d\n", temperature);
	}
}

int32_t tempreader_get_temperature(void)
{
	return temperature;
}
