/*
 * bcm2835_systimer.h
 *
 *  Created on: 13 Nov 2017
 *      Author: leonard.bise
 */

#ifndef FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_SYSTIMER_H_
#define FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_SYSTIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	_SYSTIMER0,
	_SYSTIMER1,
	_SYSTIMER2,
	_SYSTIMER3,
} e_bcm2835_timers;

bool bcm2835_systimer_matched(e_bcm2835_timers timer);

void bcm2835_systimer_clear(e_bcm2835_timers timer);

void bcm2835_systimer_setcompare(e_bcm2835_timers timer, uint32_t value);

uint32_t bcm2835_systimer_getlowcnt();

uint32_t bcm2835_systimer_gethighcnt();

#endif /* FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_SYSTIMER_H_ */
