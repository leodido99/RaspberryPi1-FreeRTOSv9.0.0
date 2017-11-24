/**
 *	Tiny Interrupt Manager
 *
 *	@author James Walmsley <james@fullfat-fs.co.uk>
 *	This code is licensed under the GNU GPLv3 license.
 *	Refactored by Léonard Bise
 **/

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

typedef void (*FN_INTERRUPT_HANDLER) (uint32_t irq, void *pParam);

typedef struct {
	FN_INTERRUPT_HANDLER 	pfnHandler;			///< Function that handles this IRQn
	void 				   *pParam;				///< A special parameter that the use can pass to the IRQ.
} INTERRUPT_VECTOR;

typedef enum {
	IRQ_SYSTIMER_0 = 0,
	IRQ_SYSTIMER_1,
	IRQ_SYSTIMER_2,
	IRQ_SYSTIMER_3,
	IRQ_ARM_TIMER = 64,
	IRQ_MAILBOX,
	IRQ_DOORBELL0,
	IRQ_DOORBELL1
} t_bcm2835_irqs;

void bcm2835_irq_register(const uint32_t irq, FN_INTERRUPT_HANDLER pfnHandler, void *pParam);
void bcm2835_irq_enable(const uint32_t irq);
void bcm2835_irq_disable(const uint32_t irq);
void bcm2835_irq_block(void);
void bcm2835_irq_unblock(void);

#endif
