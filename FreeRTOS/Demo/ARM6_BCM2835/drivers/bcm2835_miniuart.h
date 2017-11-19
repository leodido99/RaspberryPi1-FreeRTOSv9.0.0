/*
 * bcm2835_miniuart.h
 *
 *  Created on: 16 Nov 2017
 *      Author: leonard.bise
 */
#ifndef FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_MINIUART_H_
#define FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_MINIUART_H_

#include <stdint.h>
#include <stdbool.h>

void bcm2835_miniuart_open();

void bcm2835_miniuart_sendchar(char c);

void bcm2835_miniuart_enableRX(bool enable);

void bcm2835_miniuart_enableTX(bool enable);

#endif /* FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_MINIUART_H_ */
