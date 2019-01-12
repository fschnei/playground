/*
 * hal.h
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#ifndef _HAL_H_
#define _HAL_H_


#include "arm_timer/rpi-armtimer.h"
#include "auxiliaries/rpi-aux.h"
#include "gpio/rpi-gpio.h"
#include "interrupt/rpi-interrupts.h"
#include "interrupt/rpi_interrupts.h"
#include "sys_timer/rpi-systimer.h"

void hal_init(void);


#endif /* _HAL_H_ */
