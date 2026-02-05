/*
 * pl_config.h
 *
 *  Created on: 25 may. 2022
 *      Author: Asier Bolinaga
 */

#ifndef PL_CONFIG_H_
#define PL_CONFIG_H_

/* portablility layer modules activation defines */
#define PL_I2C
#define PL_THREAD
#define PL_SOCKET
//#define PL_IO_EVENT
//#define PL_SPI
#define PL_DEBUG
//#define PL_EVENT
//#define PL_GPIO
//#define PL_QUEUE
//#define PL_TIME
//#define PL_TIMER

/* used OS define */
// #define PL_OS_FREE_RTOS
#define PL_LINUX
//#define PL_WINDOWS

/* Platform defines*/
// #define PL_IMX_RT10XX
#define PL_PC
// #define PL_BEAGLEBONE

//#define PL_PTPD_TIME

#endif /* PL_CONFIG_H_ */
