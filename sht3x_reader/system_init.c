/*
 * thread_spawn.c
 *
 *  Created on: 28 ene. 2022
 *      Author: Asier Bolinaga
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board_types.h"
#include "comm.h"
#include "debug_console.h"
#include "tempreader.h"
#include "pl_hw_config.h"
#include "pl_thread.h"
#include "pl_types.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TEMP_TASK_PRIORITY 		7
#define TEMP_TASK_STACK_SIZE 	500

#define COMM_TASK_PRIORITY 		6
#define COMM_TASK_STACK_SIZE 	500

/*******************************************************************************
 * Variables
 ******************************************************************************/

static tempreader_config_t tempreader_hw_config =
{
	PL_I2C_TEMP
};

static tempreader_thread_config_t temperature_config =
{
	&tempreader_hw_config,
	false
};

static pl_thread_t temperature_thread;

static comm_thread_config_t comm_thread_config =
{
	PL_SOCKET_COMM,
	false
};

static pl_thread_t pl_comm_thread;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void system_init_threads(void);
static void system_init_thread_spawn(void);


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Funtion in charge of spawning all the
 * 		  threads of the system
 *
 */
void system_initialize(void *arg)
{
	system_init_threads();
	system_init_thread_spawn();

	pl_thread_delete();
}

static void system_init_threads(void)
{
	if(!tempreader_task_initialize(&temperature_config))
	{
		print_console("Temperature task initialization failed!.\r\n");
	}

	if(!comm_task_initialize(&comm_thread_config))
	{
		print_console("Communication task initialization failed!.\r\n");
	}
}

static void system_init_thread_spawn(void)
{
	if (PL_THREAD_RV_OK != pl_thread_create(&temperature_thread, "Temp task", tempreader_task,
	 				       TEMP_TASK_PRIORITY, TEMP_TASK_STACK_SIZE, NULL) != PL_THREAD_RV_OK)
	{
		print_console("Temperature task creation failed!.\r\n");
		while(1);
	}

	if (pl_thread_create(&pl_comm_thread, "Communication task", comm_task,
	 				      COMM_TASK_PRIORITY, COMM_TASK_STACK_SIZE, NULL) != PL_THREAD_RV_OK)
	{
		print_console("Communication task creation failed!.\r\n");
	}
}
