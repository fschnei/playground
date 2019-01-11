/*
 * rpi_interrupts.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */



#include "rpi_interrupts.h"
//#include "bcm2835_intc.h"

#include "rpi-armtimer.h"
#include "rpi-aux.h"


static INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];


typedef struct {
	unsigned long	IRQBasic;	// Pending 0
	unsigned long	Pending1;
	unsigned long	Pending2;
	unsigned long	FIQCtrl;
	unsigned long	Enable1;
	unsigned long	Enable2;
	unsigned long	EnableBasic;
	unsigned long	Disable1;
	unsigned long	Disable2;
	unsigned long	DisableBasic;
} BCM2835_INTC_REGS;

static volatile BCM2835_INTC_REGS * const pRegs = (BCM2835_INTC_REGS *) (BCM2835_BASE_INTC);

// Remember which interrupts have been enabled:
static unsigned long enabled[3];




//typedef void (*FN_INTERRUPT_HANDLER) (unsigned int irq, void *pParam);

void Timer_ISR_function(unsigned int irq, void *pParam)
{
    RPI_GetArmTimer()->IRQClear = 1;
    RPI_AuxMiniUartWrite( 'A' );
}

void irq_init(void)
{
	// register isr

	irqEnable(BCM2835_IRQ_ID_TIMER_0);

	// void irqRegister (const unsigned int irq, FN_INTERRUPT_HANDLER pfnHandler, void *pParam)
	irqRegister (BCM2835_IRQ_ID_TIMER_0, Timer_ISR_function, 0);

}





// is been called on an hw interrupt, this function calls self defined interrupt functions via function pointer array
static void handleRange (unsigned long pending, const unsigned int base)
{
	while (pending)
	{
		// Get index of first set bit:
		unsigned int bit = 31 - __builtin_clz(pending);	// count leading zeros

		// Map to IRQ number:
		unsigned int irq = base + bit;

		// Call interrupt handler, if enabled:
		if (g_VectorTable[irq].pfnHandler)
			g_VectorTable[irq].pfnHandler(irq, g_VectorTable[irq].pParam);

		// Clear bit in bitfield:
		pending &= ~(1UL << bit);
	}
}

/**
 *	This is the global IRQ handler on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
void irqHandler (void)
{
	register unsigned long ulMaskedStatus = pRegs->IRQBasic;

	//Timer_ISR_function(0, 0);

	// Bit 8 in IRQBasic indicates interrupts in Pending1 (interrupts 31-0):
	if (ulMaskedStatus & (1UL << 8))
		handleRange(pRegs->Pending1 & enabled[0], 0);

	// Bit 9 in IRQBasic indicates interrupts in Pending2 (interrupts 63-32):
	if (ulMaskedStatus & (1UL << 9))
		handleRange(pRegs->Pending2 & enabled[1], 32);

	// Bits 7 through 0 in IRQBasic represent interrupts 64-71:
	if (ulMaskedStatus & 0xFF)
		handleRange(ulMaskedStatus & 0xFF & enabled[2], 64);
}

void irqUnblock (void)
{
	asm volatile ("cpsie i" ::: "memory");
}

void irqBlock (void)
{
	asm volatile ("cpsid i" ::: "memory");
}

void irqRegister (const unsigned int irq, FN_INTERRUPT_HANDLER pfnHandler, void *pParam)
{
	if (irq < BCM2835_INTC_TOTAL_IRQ) {
		irqBlock();
		g_VectorTable[irq].pfnHandler = pfnHandler;
		g_VectorTable[irq].pParam     = pParam;
		irqUnblock();
	}
}

void irqEnable (const unsigned int irq)
{
	unsigned long mask = 1UL << (irq % 32);

	if (irq <= 31) {
		pRegs->Enable1 = mask;
		enabled[0] |= mask;
	}
	else if (irq <= 63) {
		pRegs->Enable2 = mask;
		enabled[1] |= mask;
	}
	else if (irq < BCM2835_INTC_TOTAL_IRQ) {
		pRegs->EnableBasic = mask;
		enabled[2] |= mask;
	}
}

void irqDisable (const unsigned int irq)
{
	unsigned long mask = 1UL << (irq % 32);

	if (irq <= 31) {
		pRegs->Disable1 = mask;
		enabled[0] &= ~mask;
	}
	else if (irq <= 63) {
		pRegs->Disable2 = mask;
		enabled[1] &= ~mask;
	}
	else if (irq < BCM2835_INTC_TOTAL_IRQ) {
		pRegs->DisableBasic = mask;
		enabled[2] &= ~mask;
	}
}


