/*
 * bcm2835_auxiliary.h
 *
 *  Created on: 16 Nov 2017
 *      Author: leonard.bise
 */

#ifndef FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_AUXILIARY_H_
#define FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_AUXILIARY_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	_MINIUART,
	_SPI1,
	_SPI2
} e_bcm2835_devices;

/**
 * Returns whether a device has a pending interruption
 */
bool bcm2835_aux_pendingirq(e_bcm2835_devices device);

/**
 * Enables an auxiliary device
 */
void bcm2835_aux_enable(e_bcm2835_devices device, bool enable);

#endif /* FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_AUXILIARY_H_ */
