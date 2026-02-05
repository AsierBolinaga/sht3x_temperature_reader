/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "board.h"
#include "pin_mux.h"
#include "lwipopts.h"

#include "watchdog.h"
#include "system_init.h"

#include "debug_console.h"

#include "pl_thread.h"

#include "board_config.h"

#include "fsl_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INIT_THREAD_PRIO 		DEFAULT_THREAD_PRIO
#define INIT_THREAD_STACKSIZE 	1024

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static pl_thread_t system_init_thread;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

/*!
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
    SCB_DisableDCache();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    /* pull up the ENET_INT before RESET. */
	GPIO_WritePinOutput(GPIO2, 14, 0);	// PHY_RST
	SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
	GPIO_WritePinOutput(GPIO2, 14, 1);

	mdio_init();

    print_console("\r\n******** System Start ********\r\n");

    watchdog_reset_check();
    //	watchdog_disable();

    if (PL_THREAD_RV_OK != pl_thread_create(&system_init_thread, "System init", system_initialize,
    		               INIT_THREAD_PRIO, INIT_THREAD_STACKSIZE, NULL))
	{
		print_console("System init task creation failed!.\r\n");
	}

	pl_thread_run();

	while (1)
	{
		/* ERROR: endless loop: HardFault? */
	}

	return 0;
}
