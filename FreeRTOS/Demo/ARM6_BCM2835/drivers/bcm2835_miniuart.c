/*
 * bcm2835_miniuart.c
 *
 *  Created on: 16 Nov 2017
 *      Author: leonard.bise
 */
#include "bcm2835_miniuart.h"
#include "bcm2835.h"
#include "bcm2835_auxiliary.h"

/* Both UART use the system clock which is 250Mhz */


/* In the documentation Mini UART is referred to as UART1
 * The other UART is UART0.
 * GPIO14 -> TXD0 -> Alternate Function 0
 * GPIO15 -> RXD0 -> Alternate Function 0
 * GPIO14 -> TXD1 -> Alternate Function 5
 * GPIO15 -> RXD1 -> Alternate Function 5
 * GPIO32 -> TXD0 -> Alternate Function 3
 * GPIO33 -> RXD0 -> Alternate Function 3
 * GPIO32 -> TXD1 -> Alternate Function 5
 * GPIO33 -> RXD1 -> Alternate Function 5
 * GPIO36 -> TXD0 -> Alternate Function 2
 * GPIO37 -> RXD0 -> Alternate Function 2
 * GPIO40 -> TXD1 -> Alternate Function 5
 * GPIO41 -> RXD1 -> Alternate Function 5 */
/* On the Raspberry PI only UART0 is accessible on the GPIO pins 14 and 15 */
#define _BCM2835_RPI1_TX_PIN 14
#define _BCM2835_RPI1_RX_PIN 15

#define _BCM2835_MINIUART_BASE 0x20215040

typedef struct {
	uint32_t AUX_MU_IO_REG; /* 0x20215040 Mini Uart I/O Data */
	uint32_t AUX_MU_IER_REG; /* 0x20215044 Mini Uart Interrupt Enable */
	uint32_t AUX_MU_IIR_REG; /* 0x20215048 Mini Uart Interrupt Identify */
	uint32_t AUX_MU_LCR_REG; /* 0x2021504C Mini Uart Line Control */
	uint32_t AUX_MU_MCR_REG; /* 0x20215050 Mini Uart Modem Control */
	uint32_t AUX_MU_LSR_REG; /* 0x20215054 Mini Uart Line Status */
	uint32_t AUX_MU_MSR_REG; /* 0x20215058 Mini Uart Modem Status */
	uint32_t AUX_MU_SCRATCH; /* 0x2021505C Mini Uart Scratch */
	uint32_t AUX_MU_CNTL_REG; /* 0x20215060 Mini Uart Extra Control */
	uint32_t AUX_MU_STAT_REG; /* 0x20215064 Mini Uart Extra Status */
	uint32_t AUX_MU_BAUD_REG; /* 0x20215068 Mini Uart Baudrate */
} _bcm2835_uart1_regs;

/* AUX_MU_CNTL_REG */
#define _BCM2835_CNTL_RX_ENABLE 1
#define _BCM2835_CNTL_TX_ENABLE 0

/* AUX_MU_LSR_REG */
#define _BCM2835_LSR_
#define _BCM2835_LSR_TX_IDLE 6
#define _BCM2835_LSR_TX_EMPTY 5
#define _BCM2835_LSR_RECV_OVERRUN 1
#define _BCM2835_LSR_DATA_RDY 0

typedef struct {
	uint32_t DR; /* Data Register */
	uint32_t RSRECR;
	uint32_t reserved0; /* 0x08 */
	uint32_t reserved1; /* 0x0C */
	uint32_t reserved2; /* 0x10 */
	uint32_t reserved3; /* 0x14 */
	uint32_t FR; /* Flag Register */
	uint32_t reserved4; /* 0x1C */
	uint32_t ILPAR; /* Not used */
	uint32_t IBRD; /* Integer Baud Rate Divisor */
	uint32_t FBRD; /* Fractional Baud Rate Divisor */
	uint32_t LCRH; /* Line Control Register */
	uint32_t CR; /* Control Register */
	uint32_t IFLS; /* Interrupt FIFO Level Select Register */
	uint32_t IMSC; /* Interrupt Mask Set Register */
	uint32_t RIS; /* Raw Interrupt Status Register */
	uint32_t MIS; /* Masked Interrupt Status Register */
	uint32_t ICR; /* Interrupt Clear Register */
	uint32_t DMACR; /* DMA Control Register */
	uint32_t ITCR; /* Test Control Register */
	uint32_t ITIP; /* Integration Test Input Register */
	uint32_t ITOP; /* Integration Test Output Register */
	uint32_t TDR; /* Test Data Reg */
} _bcm2835_uart0_regs;

static volatile _bcm2835_uart1_regs* pMiniUARTRegs = (_bcm2835_uart1_regs*)_BCM2835_MINIUART_BASE;

/* From documentation:
 * Baud Rate = SysClock / (8 * (Baud Register + 1))
 * For the maximum Baud Rate of 115200 Bauds:
 * => Baud Register = (250000000 - 8 * 115200)/(8 * 115200) = 270.26
 */
#define _BCM2735_UART1_BAUDRATE 270

static void configure_miniuart() {
	pMiniUARTRegs->AUX_MU_IER_REG = 0;
	/* Disable RX/TX so we can configure the UART */
	pMiniUARTRegs->AUX_MU_CNTL_REG = 0;
	/* Enable 8 bit mode (documentation is wrong, need both bit 0 and 1 to 1 to
	 * enable 8 bit) */
	pMiniUARTRegs->AUX_MU_LCR_REG = 3;
	pMiniUARTRegs->AUX_MU_MCR_REG = 0;
	pMiniUARTRegs->AUX_MU_IER_REG = 0;
	pMiniUARTRegs->AUX_MU_IIR_REG = 0xC6;
	pMiniUARTRegs->AUX_MU_BAUD_REG = _BCM2735_UART1_BAUDRATE;
}

void bcm2835_miniuart_open() {
	/* Setup GPIO pins */
	bcm2835_gpio_fsel(_BCM2835_RPI1_TX_PIN, BCM2835_GPIO_FSEL_ALT5);
	bcm2835_gpio_fsel(_BCM2835_RPI1_RX_PIN, BCM2835_GPIO_FSEL_ALT5);
	/* Enable device */
	bcm2835_aux_enable(_MINIUART, true);
	/* Configure MiniUART */
	configure_miniuart();
	bcm2835_miniuart_enableRX(true);
	bcm2835_miniuart_enableTX(true);
}

/* Returns true if transmitter FIFO can accept at least one byte */
bool is_transmitter_empty() {
	return (pMiniUARTRegs->AUX_MU_LSR_REG & (1 << _BCM2835_LSR_TX_EMPTY)) ? true : false;
}

bool is_data_ready() {
	return (pMiniUARTRegs->AUX_MU_LSR_REG & (1 << _BCM2835_LSR_DATA_RDY)) ? true : false;
}

void bcm2835_miniuart_sendchar(char c) {
	while(!is_transmitter_empty());
	/* Write symbol to transmit register */
	pMiniUARTRegs->AUX_MU_IO_REG = c;
}

void bcm2835_miniuart_sendstr(char *str) {
	uint32_t idx = 0;
	while(str[idx] != '\0') {
		bcm2835_miniuart_sendchar(str[idx]);
		idx++;
	}
}

void bcm2835_miniuart_send_blocking(const void *buf, size_t count) {
	const uint8_t* ptr = buf;
	size_t actual = 0;
	for(actual = 0; actual < count; actual++) {
		while(!is_transmitter_empty());
		pMiniUARTRegs->AUX_MU_IO_REG = *ptr;
		ptr++;
	}
}

void bcm2835_miniuart_receivechar(char* c) {
	while(!is_data_ready());
	*c = pMiniUARTRegs->AUX_MU_IO_REG;
}

void bcm2835_miniuart_receive_blocking(void *buf, size_t count) {
	uint8_t* ptr = buf;
	size_t actual = 0;
	for(actual = 0; actual < count; actual++) {
		while(!is_data_ready());
		*ptr = pMiniUARTRegs->AUX_MU_IO_REG;
		ptr++;
	}
}

void bcm2835_miniuart_enableRX(bool enable) {
	volatile uint32_t* pReg = &pMiniUARTRegs->AUX_MU_CNTL_REG;
	if (enable) {
		*pReg |= (1 << _BCM2835_CNTL_RX_ENABLE);
	} else {
		*pReg &= ~(1 << _BCM2835_CNTL_RX_ENABLE);
	}
}

void bcm2835_miniuart_enableTX(bool enable) {
	volatile uint32_t* pReg = &pMiniUARTRegs->AUX_MU_CNTL_REG;
	if (enable) {
		*pReg |= (1 << _BCM2835_CNTL_TX_ENABLE);
	} else {
		*pReg &= ~(1 << _BCM2835_CNTL_TX_ENABLE);
	}
}


