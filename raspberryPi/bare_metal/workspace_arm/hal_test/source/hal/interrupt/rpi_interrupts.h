/*
 * rpi_interrups.h
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#ifndef _RPI_INTERRUPS_H_
#define _RPI_INTERRUPS_H_

#include "../../int_types.h"


//#define BCM2835_INTC_TOTAL_IRQ		64 + 8
// 64 in pending register 0 and 1, the first 8 of basic interrupt register
#define HAL_INTERRUPT_ISR_COUNT		64 + 8

//#define BCM2835_BASE_INTC			(0x2000B200)
//#define BCM2835_BASE_INTC			(0x3F00B200)
#define HAL_INTERRUPT_BASE			(HAL_RPI_PERIPHERAL_BASE + 0x0000B200UL)
/*#define BCM2835_INTC_IRQ_BASIC		(BCM2835_BASE_INTC + 0x00)
#define BCM2835_IRQ_PENDING1		(BCM2835_BASE_INTC + 0x04)
#define BCM2835_IRQ_PENDING2		(BCM2835_BASE_INTC + 0x08)
#define BCM2835_IRQ_FIQ_CTRL		(BCM2835_BASE_INTC + 0x0C)
#define BCM2835_IRQ_ENABLE1			(BCM2835_BASE_INTC + 0x10)
#define BCM2835_IRQ_ENABLE2			(BCM2835_BASE_INTC + 0x14)
#define BCM2835_IRQ_ENABLE_BASIC	(BCM2835_BASE_INTC + 0x18)
#define BCM2835_IRQ_DISABLE1		(BCM2835_BASE_INTC + 0x1C)
#define BCM2835_IRQ_DISABLE2		(BCM2835_BASE_INTC + 0x20)
#define BCM2835_IRQ_DISABLE_BASIC	(BCM2835_BASE_INTC + 0x24)*/


// page 112 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
// pending register indicated all other interrupts which are not shown by basicPending register
typedef struct {
	volatile uint32_t	IRQBasicPending;	// Pending (2) basic: Bit8->pending1 interrupt ; bit9->pending2 interrupt
	volatile uint32_t	IRQPending1;		// Pending 0
	volatile uint32_t	IRQPending2;		// Pending 1
	volatile uint32_t	FIQCtrl;			// fast interrupt selection
	volatile uint32_t	EnableIRQs1;		// Enable 0
	volatile uint32_t	EnableIRQs2;		// Enable 1
	volatile uint32_t	EnableBasicIRQs;	// Enable (2) basic interrupts
	volatile uint32_t	DisableIRQs1;		// Disable 0
	volatile uint32_t	DisableIRQs2;		// Disable 1
	volatile uint32_t	DisableBasicIRQs;	// Disable (2) basic interrupts
} hal_interrupt_regs_t;


typedef enum {
	HAL_INTERRUPT_ID_AUX 				= 29,
	// GPU interrupt dot not enable
	HAL_INTERRUPT_ID_SPI_SLAVE			= 43,
	// GPU interrupt dot not enable
	HAL_INTERRUPT_ID_PWA0				= 45,
	HAL_INTERRUPT_ID_PWA1 				= 46,
	// GPU interrupt dot not enable
	HAL_INTERRUPT_ID_SMI 				= 48,
	HAL_INTERRUPT_ID_GPIO_0 			= 49,
	HAL_INTERRUPT_ID_GPIO_1 			= 50,
	HAL_INTERRUPT_ID_GPIO_2 			= 51,
	HAL_INTERRUPT_ID_GPIO_3 			= 52,
	HAL_INTERRUPT_ID_I2C 				= 53,
	HAL_INTERRUPT_ID_SPI 				= 54,
	HAL_INTERRUPT_ID_PCM				= 55,
	// GPU interrupt dot not enable
	HAL_INTERRUPT_ID_UART				= 57,
	// end of pending register interrupts
	// now direct irg basic register interrupts
	HAL_INTERRUPT_ID_TIMER_0			= 64,
	HAL_INTERRUPT_ID_MAILBOX_0			= 65,
	HAL_INTERRUPT_ID_DOORBELL_0			= 66,
	HAL_INTERRUPT_ID_DOORBELL_1			= 67,
	HAL_INTERRUPT_ID_GPU0_HALTED		= 68,
	HAL_INTERRUPT_ID_GPU1_HALTED		= 69,
	HAL_INTERRUPT_ID_ILLEGAL_ACCESS_1	= 70,
	HAL_INTERRUPT_ID_ILLEGAL_ACCESS_0	= 71,
} hal_interrupt_source_t;
/*
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
#define BCM2835_IRQ_ID_UART 		57*/

/*
#define BCM2835_IRQ_ID_TIMER_0 		64
#define BCM2835_IRQ_ID_MAILBOX_0	65
#define BCM2835_IRQ_ID_DOORBELL_0 	66
#define BCM2835_IRQ_ID_DOORBELL_1 	67
#define BCM2835_IRQ_ID_GPU0_HALTED	68
*/


/*#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)*/


//
typedef struct {
	uint32_t Pending;
} hal_interrupt_parms_t;



// create function pointer type for interrupt service routine callbacks
// every isr function should be this type
typedef void (*hal_interrupt_isr_t) (unsigned int irq, void *pParam);

typedef struct {
	hal_interrupt_isr_t 	pfnHandler;			///< Function that handles this IRQn
	void 				   *pParam;				///< A special parameter that the use can pass to the IRQ.
} hal_interrupt_isrPlusParms_t;


volatile hal_interrupt_regs_t * RPI_GetIrqController( void );


//void irq_init(void);
void hal_interrupt_init(void);

void hal_interrupt_isr(void);

void hal_interrupt_resgisterIsr (const unsigned int irq, hal_interrupt_isr_t pfnHandler, void *pParam, unsigned int Enable);
void hal_interrupt_enable		(const unsigned int irq);
void hal_interrupt_disable		(const unsigned int irq);
void hal_interrupt_block		(void);
void hal_interrupt_unblock		(void);



#endif /* _RPI_INTERRUPS_H_ */
