/*
 * energy.h
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "board_types.h"
#include "pl_types.h"


/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Type definitions
 ******************************************************************************/
typedef struct tempreader_thread_config
{
	tempreader_config_t*	temp_module_config;
	bool					tempreader_initialized;
}tempreader_thread_config_t;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
bool tempreader_task_initialize(tempreader_thread_config_t* _tempreader_task_config);

void tempreader_task(void* arg);

int32_t tempreader_get_temperature(void);

#endif /* TEMPERATURE_H_ */
