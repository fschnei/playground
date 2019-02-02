/*
 * rpi_interrupts.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */



#include "rpi_interrupts.h"

#include "../arm_timer/rpi_armTimer.h"
#include "../auxiliaries/rpi_aux.h"
#include "../gpio/rpi_gpio.h"
#include "../hwBasic/rpi_hwBasic.h"




extern void hal_interrupt_initIVT(void);

void hal_interrupt_resgisterIsr (const unsigned int irq, hal_interrupt_isr_t pfnHandler, void *pParam, unsigned int Enable);
void hal_interrupt_enableIRQ	(const unsigned int irq);
// this type holds the callback address of the isr and a pointer to a parameter

// add a isr of this type by calling hal_interrupt_registerAndEnableIsr
typedef struct {
	hal_interrupt_isr_t 	pfnHandler;			// callback function of the isr
	void 				   *pParam;				// pointer to parameter for isr
} hal_interrupt_isrPlusParms_t;


// 0...31 -> pending, enable, disable 0 registers
// 32..63 -> pending, enable, disable 1 registers
// 64..73 -> pending, enable, disable basic (2) registers
static hal_interrupt_isrPlusParms_t hal_interrupt_isrArray[HAL_INTERRUPT_ISR_COUNT];





//static volatile hal_interrupt_regs_t * const pRegs = (hal_interrupt_regs_t *) (HAL_INTERRUPT_BASE);
static volatile hal_interrupt_regs_t * const hal_interrupt_regs = (hal_interrupt_regs_t *) (HAL_INTERRUPT_BASE);

// remember which interrupts have been enabled (in pending register (0,1) and basic pending register (2)):
static unsigned long enabled[3];




volatile hal_interrupt_regs_t * hal_interrupt_GetRegs( void )
{
    return hal_interrupt_regs;
}


hal_base_t hal_interrupt_chackIrqFlag()
{
	hal_base_t cpsr = hal_hwBasic_GetCPSR();

	hal_base_t buffer = ((cpsr >> 4) & 1);

	return buffer;
}



void Timer_ISR_function(unsigned int irq, void * pParam)
{
	hal_armTimer_GetRegs()->IrqClear = 1;
	hal_auxiliaries_MiniUartWrite( 'A' );
}

void GPIO_ISR_function(unsigned int irq, void * pParam)
{
	hal_interrupt_parms_t IntParms;
	hal_v_base_t PinNum, PinLevel;
	hal_v_base_t PinEventDetectStatus_0, PinEventDetectStatus_1;
	hal_gpio_level_t PinValue;

	IntParms = *((hal_interrupt_parms_t*)pParam);



	//Pin = IntParms.Pending;
	//unsigned int reg;
	//rpi_volatile_reg_t reg;

	// check which is the source
	//reg = GET32(RPI_GetGpio()->GPEDS0);

	//reg = hal_gpio_getBase()->GPEDS[0];

	//PUT32(RPI_GetGpio()->GPEDS0, 0x0000);

	//hal_gpio_getBase()->GPEDS[0] = 0xFFFFFFFFUL;

	//reg = GET32(RPI_GetGpio()->GPEDS0);
	// test if cleared

	//reg = hal_gpio_getBase()->GPEDS[0];

	// get pin which interrupted
	PinEventDetectStatus_0 = hal_gpio_GetRegs()->GPEDS[0];
	PinEventDetectStatus_1 = hal_gpio_GetRegs()->GPEDS[1];

	if(PinEventDetectStatus_0 != 0)
	{
		PinNum = 31 - __builtin_clz(PinEventDetectStatus_0);
	}
	else if(PinEventDetectStatus_1 != 0)
	{
		PinNum = 31 - __builtin_clz(PinEventDetectStatus_1);
	}
	else
	{
		// set Pin to error state
		PinNum = 0xFFFFFFFFUL;
	}
	if(PinNum <= 53) // highest gpio pin number
	{
		// get level of pin which interrupted
		 hal_gpio_GetValue( PinNum, &PinValue );

		// get edge
		if (PinValue == HAL_GPIO_LVL_HI)
		{
			// rising edge
		}
		else
		{
			// falling edge
		}
	}
}


void hal_interrupt_Init(void)
{
	// register isr

	hal_interrupt_initIVT();

	hal_interrupt_RegisterAndEnableIsr (HAL_INTERRUPT_ID_TIMER_0, Timer_ISR_function, 0, 0);

	hal_interrupt_RegisterAndEnableIsr (HAL_INTERRUPT_ID_GPIO_0, GPIO_ISR_function, 0, 0);

}





// is been called on an hw interrupt, this function calls self defined interrupt functions via function pointer array
static void hal_interrupt_isr_handleRange (unsigned long pending, const unsigned int base)
{
	while (pending)
	{
		// copy pending register to function parms
		//hal_intParms.Pending = pending;

		// Get index of first set bit:
		unsigned int bit = 31 - __builtin_clz(pending);	// count leading zeros

		// Map to IRQ number:
		unsigned int irq = base + bit;

		// Call interrupt handler, if enabled:
		if (hal_interrupt_isrArray[irq].pfnHandler)
			hal_interrupt_isrArray[irq].pfnHandler(irq, hal_interrupt_isrArray[irq].pParam);

		// Clear bit in bitfield:
		pending &= ~(1UL << bit);
	}
}

/**
 *	This is the global IRQ handler on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
void hal_interrupt_isr (void)
{
	// page 112 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
	register unsigned long ulMaskedStatus = hal_interrupt_regs->IRQBasicPending;

	//

	// Bit 8 in IRQBasic indicates interrupts in Pending1 (interrupts 0-31):
	if (ulMaskedStatus & (1UL << 8))
		hal_interrupt_isr_handleRange(hal_interrupt_regs->IRQPending1 & enabled[0], 0);

	// Bit 9 in IRQBasic indicates interrupts in Pending2 (interrupts 32-63):
	if (ulMaskedStatus & (1UL << 9))
		hal_interrupt_isr_handleRange(hal_interrupt_regs->IRQPending2 & enabled[1], 32);

	// Bits 7 through 0 in IRQBasic represent interrupts 64-71:
	// 0	64	ARM Timer
	// 1	65	ARM Mailbox
	// 2	66	ARM Doorbell 0
	// 3	67	ARM Doorbell 1
	// 4	68	GPU 0 halted
	// 5	69	GPU 1 halted
	// 6	70	Illegal access type 1
	// 7	71	Illegal access type 0
	if (ulMaskedStatus & 0xFF)
		hal_interrupt_isr_handleRange(ulMaskedStatus & 0xFF & enabled[2], 64);


	// clear all remaining interrupt flags -> interrupt with no handler will not rise in series and block cpu

	// debug: clear timer flag:

	hal_armTimer_GetRegs()->IrqClear = 1;

	hal_armTimer_ClearIrq();

    // gpio interrupts are clear by write a 1 to corresponding register (see p. 96)
    // clear pending gpio bits 0 ... 31
	hal_gpio_GetRegs()->GPEDS[0] = 0xFFFFFFFFUL;
    // clear pending gpio bits 32 ... 53
	hal_gpio_GetRegs()->GPEDS[1] = 0x001FFFFFUL;

	// TODO: clear all other possibly occurring interrupts

}

void hal_interrupt_UnblockIrq (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsie i" ::: "memory");
}

void hal_interrupt_BlockIrq (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsid i" ::: "memory");
}

void hal_interrupt_resgisterIsr (const unsigned int irq, hal_interrupt_isr_t pfnHandler, void *pParam, unsigned int Enable)
{
	if (irq < HAL_INTERRUPT_ISR_COUNT) {
		hal_interrupt_BlockIrq();
		hal_interrupt_isrArray[irq].pfnHandler = pfnHandler;
		hal_interrupt_isrArray[irq].pParam     = pParam;
		if(Enable)
			hal_interrupt_UnblockIrq();
	}
}

void hal_interrupt_RegisterAndEnableIsr (const unsigned int irq, hal_interrupt_isr_t pfnHandler, void *pParam, unsigned int Enable)
{
	hal_interrupt_resgisterIsr (irq, pfnHandler, pParam, 0);
	hal_interrupt_enableIRQ(irq);
	if(Enable)
		hal_interrupt_UnblockIrq();
}

void hal_interrupt_enableIRQ (const unsigned int irq)
{
	// write bit into enable register -> interrupt can occur in pending registers
	unsigned long mask = 1UL << (irq % 32);
	hal_base_t irqFlag;

	if (irq < HAL_INTERRUPT_ISR_COUNT)
	{
		// check if interrupt are enabled
		irqFlag = hal_interrupt_chackIrqFlag();
		// disable interrupts
		hal_interrupt_BlockIrq();

		// check which register to write to
		if (irq <= 31)
		{
			hal_interrupt_regs->EnableIRQs1 = mask;
			enabled[0] |= mask;
		}
		else if (irq <= 63)
		{
			hal_interrupt_regs->EnableIRQs2 = mask;
			enabled[1] |= mask;
		}
		else if (irq < HAL_INTERRUPT_ISR_COUNT)
		{
			hal_interrupt_regs->EnableBasicIRQs = mask;
			enabled[2] |= mask;
		}

		// enable irq if interrupts was enabled
		if(irqFlag != 0)
		{
			hal_interrupt_UnblockIrq();
		}
	}


}

void hal_interrupt_DisableIRQ (const unsigned int irq)
{
	// write bit into disable register -> interrupt can not occur in pending registers
	unsigned long mask = 1UL << (irq % 32);

	if (irq <= 31) {
		hal_interrupt_regs->DisableIRQs1 = mask;
		enabled[0] &= ~mask;
	}
	else if (irq <= 63) {
		hal_interrupt_regs->DisableIRQs2 = mask;
		enabled[1] &= ~mask;
	}
	else if (irq < HAL_INTERRUPT_ISR_COUNT) {
		hal_interrupt_regs->DisableBasicIRQs = mask;
		enabled[2] &= ~mask;
	}
}





// --------------- Interrupt Vectors -----------------------
// see .start asm file


void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
 	hal_interrupt_isr();
}








