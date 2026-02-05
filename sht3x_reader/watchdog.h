/*
 * watchdog.h
 *
 *  Created on: Jan 19, 2023
 *      Author: abolinaga
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_


void watchdog_reset_check(void);
void watchdog_init(void);
void watchdog_disable(void);
void watchdog_test(void);

#endif /* WATCHDOG_H_ */
