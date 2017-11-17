/*
 * bcm2835_auxiliary.c
 *
 *  Created on: 16 Nov 2017
 *      Author: leonard.bise
 */
#include "bcm2835_auxiliary.h"

#define _BCM2835_AUXILIARY_BASE 0x20215000

typedef struct {
	uint32_t AUX_IRQ;
	uint32_t AUX_ENABLES;
} _bcm2835_aux_registers;

static volatile _bcm2835_aux_registers* pAuxRegs = (_bcm2835_aux_registers*)_BCM2835_AUXILIARY_BASE;

bool bcm2835_aux_pendingirq(e_bcm2835_devices device) {
	return pAuxRegs->AUX_IRQ & (1 << device);
}

void bcm2835_aux_enable(e_bcm2835_devices device, bool enable) {
	if (enable) {
		pAuxRegs->AUX_ENABLES |= (1 << device);
	} else {
		pAuxRegs->AUX_ENABLES &= ~(1 << device);
	}
}
