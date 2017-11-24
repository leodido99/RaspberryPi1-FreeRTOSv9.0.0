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
#include <stdlib.h>

void bcm2835_miniuart_open();

void bcm2835_miniuart_sendchar(char c);

void bcm2835_miniuart_sendstr(char *str);

void bcm2835_miniuart_send_blocking(const void *buf, size_t count);

void bcm2835_miniuart_receivechar(char* c);

void bcm2835_miniuart_receive_blocking(void *buf, size_t count);

void bcm2835_miniuart_enableRX(bool enable);

void bcm2835_miniuart_enableTX(bool enable);

#endif /* FREERTOS_DEMO_ARM6_BCM2835_DRIVERS_BCM2835_MINIUART_H_ */
