
#include "debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_wdog.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_WDOG_BASE       WDOG1
#define DEMO_WDOG_IRQHandler WDOG1_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_WDOG_IRQHandler(void)
{
    WDOG_ClearInterruptStatus(DEMO_WDOG_BASE, kWDOG_InterruptFlag);
    /* User code. User can do urgent case before timeout reset.
     * IE. user can backup the ram data or ram log to flash.
     * the period is set by config.interruptTimeValue, user need to
     * check the period between interrupt and timeout.
     */
}

/*!
 * @brief Main function
 */
void watchdog_reset_check(void)
{
    uint16_t resetFlag = 0U;

    print_console("System reset by:\n");

    resetFlag = WDOG_GetStatusFlags(WDOG1);

    switch (resetFlag & (kWDOG_PowerOnResetFlag | kWDOG_TimeoutResetFlag | kWDOG_SoftwareResetFlag))
    {
        case kWDOG_PowerOnResetFlag:
            print_console("wdog1 Power On Reset!\r\n");
            break;
        case kWDOG_TimeoutResetFlag:
        	print_console("wdog1 Time Out Reset!\r\n");
            break;
        case kWDOG_SoftwareResetFlag:
        	print_console("wdog1 Software Reset!\r\n");
            break;
        default:
        	print_console("wdog1 Error status!\r\n");
            break;
    }

    resetFlag = WDOG_GetStatusFlags(WDOG2);

    switch (resetFlag & (kWDOG_PowerOnResetFlag | kWDOG_TimeoutResetFlag | kWDOG_SoftwareResetFlag))
   	{
	   case kWDOG_PowerOnResetFlag:
		   print_console("wdog2 Power On Reset!\r\n");
		   break;
	   case kWDOG_TimeoutResetFlag:
		   print_console("wdog2 Time Out Reset!\r\n");
		   break;
	   case kWDOG_SoftwareResetFlag:
		   print_console("wdog2 Software Reset!\r\n");
		   break;
	   default:
		   print_console("wdog2 Error status!\r\n");
		   break;
    }
}

void watchdog_init(void)
{
    wdog_config_t config;
	print_console("\r\n- 3.Test the WDOG refresh function by using interrupt.\r\n");
	/*
	 * wdogConfig->enableWdog = true;
	 * wdogConfig->workMode.enableWait = true;
	 * wdogConfig->workMode.enableStop = false;
	 * wdogConfig->workMode.enableDebug = false;
	 * wdogConfig->enableInterrupt = false;
	 * wdogConfig->enablePowerdown = false;
	 * wdogConfig->resetExtension = flase;
	 * wdogConfig->timeoutValue = 0xFFU;
	 * wdogConfig->interruptTimeValue = 0x04u;
	 */
	WDOG_GetDefaultConfig(&config);
	config.timeoutValue       = 0xFU; /* Timeout value is (0xF+1)/2 = 8 sec. */
	config.enableInterrupt    = true;
	config.interruptTimeValue = 0x4U; /* Interrupt occurred (0x4)/2 = 2 sec before WDOG timeout. */
	WDOG_Init(DEMO_WDOG_BASE, &config);

	print_console("--- wdog Init done---\r\n");

	while (1)
	{
		/* User can feed WDG in their main thread. */
		WDOG_Refresh(DEMO_WDOG_BASE);
		print_console(" \r\nWDOG has be refreshed!");

		/* Delay. */
//        delay(SystemCoreClock);
	}
}



void watchdog_disable(void)
{
	WDOG_Disable(WDOG1);
	WDOG_Disable(WDOG2);
}

void watchdog_test(void)
{
    uint16_t resetFlag = 0U;
    resetFlag = WDOG_GetStatusFlags(WDOG1);
    wdog_config_t config;
	/* If system reset from power on, trigger a software reset. */
	if (resetFlag & kWDOG_PowerOnResetFlag)
	{
		print_console("\r\n- 1.Testing System reset by software trigger...   ");
		WDOG_TriggerSystemSoftwareReset(DEMO_WDOG_BASE);
	}

	/* If system reset from software trigger, testing the timeout reset. */
	if (resetFlag & kWDOG_SoftwareResetFlag)
	{
		print_console("\r\n- 2.Testing system reset by WDOG timeout.\r\n");
		/*
		 * wdogConfig->enableWdog = true;
		 * wdogConfig->workMode.enableWait = true;
		 * wdogConfig->workMode.enableStop = false;
		 * wdogConfig->workMode.enableDebug = false;
		 * wdogConfig->enableInterrupt = false;
		 * wdogConfig->enablePowerdown = false;
		 * wdogConfig->resetExtension = flase;
		 * wdogConfig->timeoutValue = 0xFFU;
		 * wdogConfig->interruptTimeValue = 0x04u;
		 */
		WDOG_GetDefaultConfig(&config);
		config.timeoutValue = 0xFU; /* Timeout value is (0xF + 1)/2 = 8 sec. */
		WDOG_Init(DEMO_WDOG_BASE, &config);
		print_console("--- wdog Init done---\r\n");

		/* without feed watch dog, wait until timeout. */
		while (1)
		{
		}
	}

   /* If system reset from WDOG timeout, testing the refresh function using interrupt. */
   if (resetFlag & kWDOG_TimeoutResetFlag)
   {

   }
}
