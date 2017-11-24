/*
 * bcm2835_systimer.c
 *
 *  Created on: 13 Nov 2017
 *      Author: leonard.bise
 *  Description:
 *  Interface to the raspberry pi 1 system timer. This timer
 *  is managed by the GPU which runs at a frequency of 1Mhz.
 *  You can have 4 different compare values which can trigg an
 *  interruption.
 */

#include "bcm2835_systimer.h"
#include "bcm2835_irq.h"
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

static uint32_t s_systimers_intervals[4] = {0,0,0,0};
static timer_irq_handler handlers[4] = {NULL, NULL, NULL, NULL};

bool bcm2835_systimer_matched(e_bcm2835_timers timer) {
	uint32_t mask = (1 << timer);
	return ((pSysTimerRegs->CS & mask) ? true : false);
}

void bcm2835_systimer_clear(e_bcm2835_timers timer) {
	uint32_t mask = (1 << timer);
	pSysTimerRegs->CS |= mask;
}

void bcm2835_set_handler(e_bcm2835_timers timer, timer_irq_handler handler) {
	if (timer < 4) {
		handlers[timer] = handler;
	}
}

static void interrupt_handler(uint32_t nIRQ, void *pParam) {
	switch(nIRQ) {
	case IRQ_SYSTIMER_0:
		/* Set timer nb tick in front of current value */
		pSysTimerRegs->C0 = (pSysTimerRegs->CLO + s_systimers_intervals[0]) % 0xFFFFFFFF;
		/* Clear timer */
		pSysTimerRegs->CS |= (1 << _SYSTIMER0);
		if (handlers[0] != NULL) {
			handlers[0](0);
		}
		break;
	case IRQ_SYSTIMER_1:
		/* Set timer nb tick in front of current value */
		pSysTimerRegs->C1 = (pSysTimerRegs->CLO + s_systimers_intervals[1]) % 0xFFFFFFFF;
		/* Clear timer */
		pSysTimerRegs->CS |= (1 << _SYSTIMER1);
		if (handlers[1] != NULL) {
			handlers[1](1);
		}
		break;
	case IRQ_SYSTIMER_2:
		/* Set timer nb tick in front of current value */
		pSysTimerRegs->C2 = (pSysTimerRegs->CLO + s_systimers_intervals[2]) % 0xFFFFFFFF;
		/* Clear timer */
		pSysTimerRegs->CS |= (1 << _SYSTIMER2);
		if (handlers[2] != NULL) {
			handlers[2](2);
		}
		break;
	case IRQ_SYSTIMER_3:
		/* Set timer nb tick in front of current value */
		pSysTimerRegs->C3 = (pSysTimerRegs->CLO + s_systimers_intervals[3]) % 0xFFFFFFFF;
		/* Clear timer */
		pSysTimerRegs->CS |= (1 << _SYSTIMER3);
		if (handlers[3] != NULL) {
			handlers[3](3);
		}
		break;
	}
}

void bcm2835_systimer_setinterval(e_bcm2835_timers timer, uint32_t value) {
	bcm2835_irq_register(timer, interrupt_handler, NULL);
	bcm2835_irq_enable(timer);
	if (timer < 4) {
		s_systimers_intervals[timer] = value;
		bcm2835_systimer_setcompareincrement(timer, value);
		bcm2835_systimer_clear(timer);
	}
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

void bcm2835_systimer_setcompareincrement(e_bcm2835_timers timer, uint32_t value) {
	bcm2835_systimer_setcompare(timer, pSysTimerRegs->CLO + value);
}

uint32_t bcm2835_systimer_getlowcnt() {
	return pSysTimerRegs->CLO;
}

uint32_t bcm2835_systimer_gethighcnt() {
	return pSysTimerRegs->CHI;
}
