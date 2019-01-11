/*
 * rpi_interrups.h
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#ifndef _RPI_INTERRUPS_H_
#define _RPI_INTERRUPS_H_



#define BCM2835_INTC_TOTAL_IRQ		64 + 8

//#define BCM2835_BASE_INTC			(0x2000B200)
#define BCM2835_BASE_INTC			(0x3F00B200)
#define BCM2835_INTC_IRQ_BASIC		(BCM2835_BASE_INTC + 0x00)
#define BCM2835_IRQ_PENDING1		(BCM2835_BASE_INTC + 0x04)
#define BCM2835_IRQ_PENDING2		(BCM2835_BASE_INTC + 0x08)
#define BCM2835_IRQ_FIQ_CTRL		(BCM2835_BASE_INTC + 0x0C)
#define BCM2835_IRQ_ENABLE1			(BCM2835_BASE_INTC + 0x10)
#define BCM2835_IRQ_ENABLE2			(BCM2835_BASE_INTC + 0x14)
#define BCM2835_IRQ_ENABLE_BASIC	(BCM2835_BASE_INTC + 0x18)
#define BCM2835_IRQ_DISABLE1		(BCM2835_BASE_INTC + 0x1C)
#define BCM2835_IRQ_DISABLE2		(BCM2835_BASE_INTC + 0x20)
#define BCM2835_IRQ_DISABLE_BASIC	(BCM2835_BASE_INTC + 0x24)




#define BCM2835_IRQ_ID_AUX			29
#define BCM2835_IRQ_ID_SPI_SLAVE 	43
#define BCM2835_IRQ_ID_PWA0			45
#define BCM2835_IRQ_ID_PWA1		   	46
#define BCM2835_IRQ_ID_SMI 			48
#define BCM2835_IRQ_ID_GPIO_0 		49
#define BCM2835_IRQ_ID_GPIO_1 		50
#define BCM2835_IRQ_ID_GPIO_2 		51
#define BCM2835_IRQ_ID_GPIO_3 		52
#define BCM2835_IRQ_ID_I2C 			53
#define BCM2835_IRQ_ID_SPI 			54
#define BCM2835_IRQ_ID_PCM 			55
#define BCM2835_IRQ_ID_UART 		57


#define BCM2835_IRQ_ID_TIMER_0 		64
#define BCM2835_IRQ_ID_MAILBOX_0	65
#define BCM2835_IRQ_ID_DOORBELL_0 	66
#define BCM2835_IRQ_ID_DOORBELL_1 	67
#define BCM2835_IRQ_ID_GPU0_HALTED	68








typedef void (*FN_INTERRUPT_HANDLER) (unsigned int irq, void *pParam);

typedef struct {
	FN_INTERRUPT_HANDLER 	pfnHandler;			///< Function that handles this IRQn
	void 				   *pParam;				///< A special parameter that the use can pass to the IRQ.
} INTERRUPT_VECTOR;


void irq_init(void);

void irqHandler (void);

void irqRegister	(const unsigned int irq, FN_INTERRUPT_HANDLER pfnHandler, void *pParam);
void irqEnable		(const unsigned int irq);
void irqDisable		(const unsigned int irq);
void irqBlock		(void);
void irqUnblock		(void);



#endif /* _RPI_INTERRUPS_H_ */
