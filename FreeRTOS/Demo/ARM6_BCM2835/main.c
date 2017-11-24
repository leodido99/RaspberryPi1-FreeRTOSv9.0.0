#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio.h>

#include "bcm2835.h"
#include "bcm2835_irq.h"
#include "bcm2835_systimer.h"
#include "bcm2835_miniuart.h"
#include "raspberrypi1.h"

SemaphoreHandle_t led1Semaphore;
SemaphoreHandle_t led2Semaphore;

void task1(void *pParam) {

	int i = 0;
	while(1) {
		i++;
		xSemaphoreTake(led1Semaphore, portMAX_DELAY);
		bcm2835_gpio_set(_RPI1_RDY_LED_PIN);
		// 1 sec
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xSemaphoreGive(led2Semaphore);
	}
}

void task2(void *pParam) {

	int i = 0;
	while(1) {
		i++;
		xSemaphoreTake(led2Semaphore, portMAX_DELAY);
		bcm2835_gpio_clr(_RPI1_RDY_LED_PIN);
		// 1 sec
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		xSemaphoreGive(led1Semaphore);
	}
}

void task3(void *pParam) {
	/* Set first display char to ! */
	char c = '!';
	while(1) {
		bcm2835_miniuart_sendchar(c);
		c++;
		if (c > 0x7E) {
			/* reset char to ! */
			c = '!';
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void timerirq(uint32_t nIRQ, void *pParam) {
	bcm2835_miniuart_sendstr("timer irq\n\r");
	bcm2835_systimer_clear(_SYSTIMER1);
	bcm2835_systimer_setcompareincrement(_SYSTIMER1, BCM2835_SYSTIMER_FREQ - 1);
}

void timerirq2(uint32_t timer) {
	printf("timerirq2 %d\n\r", (int)timer);
}

int main (void) {
	/* Initialize the bcm2835 lib */
	bcm2835_init();
	/* Initialize the miniuart (Otherwise printf doesn't work) */
	bcm2835_miniuart_open();

	/* Send message */
	printf("Welcome to FreeRTOSv%d.0.0\n\r", 9);

	/* Rdy Led as output */
	bcm2835_gpio_fsel(_RPI1_RDY_LED_PIN, BCM2835_GPIO_FSEL_OUTP);

	/* Configure a Timer */
	//bcm2835_set_handler(_SYSTIMER1, timerirq2);
	//bcm2835_systimer_setinterval(_SYSTIMER1, BCM2835_SYSTIMER_FREQ - 1);

//	bcm2835_irq_register(_SYSTIMER1, timerirq, NULL);
//	bcm2835_irq_enable(_SYSTIMER1);
//	bcm2835_systimer_clear(_SYSTIMER1);
//	/* Once per second */
//	bcm2835_systimer_setcompareincrement(_SYSTIMER1, BCM2835_SYSTIMER_FREQ - 1);

    /* Attempt to create a semaphore. */
    led1Semaphore = xSemaphoreCreateBinary();
    led2Semaphore = xSemaphoreCreateBinary();

    if( led1Semaphore == NULL || led2Semaphore == NULL ) {
        /* There was insufficient FreeRTOS heap available for the semaphore to
        be created successfully. */
    		while(1);
    }

    xSemaphoreGive(led1Semaphore);

	xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL);
	xTaskCreate(task2, "LED_1", 128, NULL, 0, NULL);
	xTaskCreate(task3, "UART", 128, NULL, 0, NULL);

	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1);
}
