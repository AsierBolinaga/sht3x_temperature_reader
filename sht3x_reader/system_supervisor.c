/*
 * system_supervisor.c
 *
 *  Created on: Mar 24, 2023
 *      Author: abolinaga
 */

#include "debug_console.h"
#include "FreeRTOS.h"
#include "task.h"


void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
	print_console("ERROR!! Stack overflow of thread %s!\n", pcTaskName);
}

