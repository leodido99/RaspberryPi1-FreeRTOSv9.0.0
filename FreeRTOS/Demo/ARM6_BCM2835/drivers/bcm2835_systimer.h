/*
 * bcm2835_systimer.h
 *
 *  Created on: 13 Nov 2017
 *      Author: leonard.bise
 *  Description:
 *  Interface to the raspberry pi 1 system timer. This timer
 *  is managed by the GPU which runs at a frequency of 1Mhz.
 *  You can have 4 different compare values which can trigg an
 *  interruption.
 */

#ifndef _BCM2835_SYSTIMER_H_
#define _BCM2835_SYSTIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	_SYSTIMER0,
	_SYSTIMER1,
	_SYSTIMER2,
	_SYSTIMER3,
} e_bcm2835_timers;

#define BCM2835_SYSTIMER_FREQ 1000000

typedef void (*timer_irq_handler) (uint32_t timer);

/**
 * Initialize the SysTimer driver
 */
void bcm2835_systimer_init();

/**
 * Configures the timer to trigger every interval
 * The driver handle the interruption
 */
void bcm2835_systimer_setinterval(e_bcm2835_timers timer, uint32_t value);

/**
 * Sets a function to be called when the timer elapses
 * The timer itself is handled by the driver no need to clear anything in the handler
 */
void bcm2835_set_handler(e_bcm2835_timers timer, timer_irq_handler handler);



bool bcm2835_systimer_matched(e_bcm2835_timers timer);

void bcm2835_systimer_clear(e_bcm2835_timers timer);

void bcm2835_systimer_setcompare(e_bcm2835_timers timer, uint32_t value);

void bcm2835_systimer_setcompareincrement(e_bcm2835_timers timer, uint32_t value);

uint32_t bcm2835_systimer_getlowcnt();

uint32_t bcm2835_systimer_gethighcnt();

#endif /* _BCM2835_SYSTIMER_H_ */
