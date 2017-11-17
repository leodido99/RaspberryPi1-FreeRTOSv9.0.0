/*
 * bcm2835_systimer.c
 *
 *  Created on: 13 Nov 2017
 *      Author: leonard.bise
 *  Description:
 *  Interface to the raspberry pi 1 system timer. This timer
 *  is managed by the GPU which runs at a frequency of 250Mhz.
 *  You can have 4 different compare values which can trigg an
 *  interruption.
 */

#include "bcm2835_systimer.h"
#include <stdlib.h>

#define _BCM2835_SYSTIMER_BASE 0x20003000

typedef struct {
	uint32_t CS;  /* System Timer Control/Status */
	uint32_t CLO; /* System Timer Counter Lower 32 bits */
	uint32_t CHI; /* System Timer Counter Higher 32 bits */
	uint32_t C0;  /* System Timer Compare 0 */
	uint32_t C1;  /* System Timer Compare 1 */
	uint32_t C2;  /* System Timer Compare 2 */
	uint32_t C3;  /* System Timer Compare 3 */

} _bcm2835_systimer_regs;

static volatile _bcm2835_systimer_regs* pSysTimerRegs = (_bcm2835_systimer_regs*)_BCM2835_SYSTIMER_BASE;

bool bcm2835_systimer_matched(e_bcm2835_timers timer) {
	uint32_t mask = (1 << timer);
	return ((pSysTimerRegs->CS & mask) ? true : false);
}

void bcm2835_systimer_clear(e_bcm2835_timers timer) {
	uint32_t mask = (1 << timer);
	pSysTimerRegs->CS |= mask;
}

void bcm2835_systimer_setcompare(e_bcm2835_timers timer, uint32_t value) {
	volatile uint32_t* pReg = NULL;
	switch(timer) {
	case _SYSTIMER0:
		pReg = &pSysTimerRegs->C0;
		break;
	case _SYSTIMER1:
		pReg = &pSysTimerRegs->C1;
		break;
	case _SYSTIMER2:
		pReg = &pSysTimerRegs->C2;
		break;
	case _SYSTIMER3:
		pReg = &pSysTimerRegs->C3;
		break;
	}
	if (pReg != NULL) {
		*pReg = value;
	}
}

uint32_t bcm2835_systimer_getlowcnt() {
	return pSysTimerRegs->CLO;
}

uint32_t bcm2835_systimer_gethighcnt() {
	return pSysTimerRegs->CHI;
}
