/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "system_init.h"

#include "board.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"




/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitModuleClock(void)
{
	const clock_enet_pll_config_t config = {true, true, 1, 0};
	CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 *
 * @brief Main function
 */
int main(void)
{
	gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

	BOARD_ConfigMPU();
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	BOARD_InitModuleClock();

	IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

	GPIO_PinInit(GPIO1, 9, &gpio_config);
	GPIO_PinInit(GPIO1, 10, &gpio_config);
	/* pull up the ENET_INT before RESET. */
	GPIO_WritePinOutput(GPIO1, 10, 1);
	GPIO_WritePinOutput(GPIO1, 9, 0);
	delay();
	GPIO_WritePinOutput(GPIO1, 9, 1);


	system_initialize();

	while (1)
	{
		/* ERROR: endless loop: HardFault? */
	}
	/* Will not get here unless a task calls vTaskEndScheduler ()*/
	return 0;
}

