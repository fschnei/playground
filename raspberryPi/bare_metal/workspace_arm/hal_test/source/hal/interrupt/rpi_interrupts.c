/*
 * rpi_interrupts.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */



#include "rpi_interrupts.h"

#include "../arm_timer/rpi-armtimer.h"
//#include "bcm2835_intc.h"

#include "../auxiliaries/rpi-aux.h"

#include "../../asm_prototypes.h"
#include "../gpio/rpi-gpio.h"


// 0...31 -> pending, enable, disable 0 registers
// 32..63 -> pending, enable, disable 1 registers
// 64..73 -> pending, enable, disable basic (2) registers
static hal_interrupt_isrPlusParms_t g_VectorTable[HAL_INTERRUPT_ISR_COUNT];





static volatile hal_interrupt_regs_t * const pRegs = (hal_interrupt_regs_t *) (HAL_INTERRUPT_BASE);

// Remember which interrupts have been enabled (in pending register (0,1) and basic pending register (2)):
static unsigned long enabled[3];


static hal_interrupt_parms_t hal_intParms;



volatile hal_interrupt_regs_t * RPI_GetIrqController( void )
{
    return pRegs;
}



//typedef void (*FN_INTERRUPT_HANDLER) (unsigned int irq, void *pParam);

void Timer_ISR_function(unsigned int irq, void *pParam)
{
    RPI_GetArmTimer()->IRQClear = 1;
    RPI_AuxMiniUartWrite( 'A' );
}

void GPIO_ISR_function(unsigned int irq, void * pParam)
{
	hal_interrupt_parms_t IntParms;
	uint32_t Pin;
	IntParms = *((hal_interrupt_parms_t*)pParam);
	Pin = IntParms.Pending;

	//unsigned int reg;
	rpi_volatile_reg_t reg;
	// check which is the source
	//reg = GET32(RPI_GetGpio()->GPEDS0);
	reg = hal_gpio_getBase()->GPEDS[0];
	//PUT32(RPI_GetGpio()->GPEDS0, 0x0000);
	hal_gpio_getBase()->GPEDS[0] = 0xFFFFFFFFUL;
	//reg = GET32(RPI_GetGpio()->GPEDS0);
	// test if cleared
	reg = hal_gpio_getBase()->GPEDS[0];
}


void hal_interrupt_init(void)
{
	// register isr
	// void irqRegister (const unsigned int irq, FN_INTERRUPT_HANDLER pfnHandler, void *pParam)

	//hal_interrupt_resgisterIsr (HAL_INTERRUPT_ID_TIMER_0, Timer_ISR_function, 0, 0);
	//hal_interrupt_enable(HAL_INTERRUPT_ID_TIMER_0);

	hal_interrupt_resgisterIsr (HAL_INTERRUPT_ID_GPIO_0, GPIO_ISR_function, (void*)(&hal_intParms), 0);
	hal_interrupt_enable(HAL_INTERRUPT_ID_GPIO_0);

}





// is been called on an hw interrupt, this function calls self defined interrupt functions via function pointer array
static void hal_interrupt_isr_handleRange (unsigned long pending, const unsigned int base)
{
	while (pending)
	{
		// copy pending register to function parms
		hal_intParms.Pending = pending;

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
void hal_interrupt_isr (void)
{
	// page 112 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
	register unsigned long ulMaskedStatus = pRegs->IRQBasicPending;

	//

	// Bit 8 in IRQBasic indicates interrupts in Pending1 (interrupts 0-31):
	if (ulMaskedStatus & (1UL << 8))
		hal_interrupt_isr_handleRange(pRegs->IRQPending1 & enabled[0], 0);

	// Bit 9 in IRQBasic indicates interrupts in Pending2 (interrupts 32-63):
	if (ulMaskedStatus & (1UL << 9))
		hal_interrupt_isr_handleRange(pRegs->IRQPending2 & enabled[1], 32);

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
    RPI_GetArmTimer()->IRQClear = 1;
    // gpio interrupts are clear by write a 1 to corresponding register (see p. 96)
    // clear pending gpio bits 0 ... 31
    hal_gpio_getBase()->GPEDS[0] = 0xFFFFFFFFUL;
    // clear pending gpio bits 32 ... 53
    hal_gpio_getBase()->GPEDS[1] = 0x001FFFFFUL;

	// TODO: clear all other possibly occurring interrupts

}

void hal_interrupt_unblock (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsie i" ::: "memory");
}

void hal_interrupt_block (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsid i" ::: "memory");
}

void hal_interrupt_resgisterIsr (const unsigned int irq, hal_interrupt_isr_t pfnHandler, void *pParam, unsigned int Enable)
{
	if (irq < HAL_INTERRUPT_ISR_COUNT) {
		hal_interrupt_block();
		g_VectorTable[irq].pfnHandler = pfnHandler;
		g_VectorTable[irq].pParam     = pParam;
		if(Enable)
			hal_interrupt_unblock();
	}
}

void hal_interrupt_enable (const unsigned int irq)
{
	unsigned long mask = 1UL << (irq % 32);

	if (irq <= 31) {
		pRegs->EnableIRQs1 = mask;
		enabled[0] |= mask;
	}
	else if (irq <= 63) {
		pRegs->EnableIRQs2 = mask;
		enabled[1] |= mask;
	}
	else if (irq < HAL_INTERRUPT_ISR_COUNT) {
		pRegs->EnableBasicIRQs = mask;
		enabled[2] |= mask;
	}
}

void hal_interrupt_disable (const unsigned int irq)
{
	unsigned long mask = 1UL << (irq % 32);

	if (irq <= 31) {
		pRegs->DisableIRQs1 = mask;
		enabled[0] &= ~mask;
	}
	else if (irq <= 63) {
		pRegs->DisableIRQs2 = mask;
		enabled[1] &= ~mask;
	}
	else if (irq < HAL_INTERRUPT_ISR_COUNT) {
		pRegs->DisableBasicIRQs = mask;
		enabled[2] &= ~mask;
	}
}



// --------------- Interrupt Vectors -----------------------



/**
    @brief The Reset vector interrupt handler

    This can never be called, since an ARM core reset would also reset the
    GPU and therefore cause the GPU to start running code again until
    the ARM is handed control at the end of boot loading
*/
void __attribute__((interrupt("ABORT"))) reset_vector(void)
{
    while( 1 )
    {
        //LED_ON();
    }
}

/**
    @brief The undefined instruction interrupt handler

    If an undefined intstruction is encountered, the CPU will start
    executing this function. Just trap here as a debug solution.
*/
void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
    while( 1 )
    {
        /* Do Nothing! */
        //LED_ON();
    }
}


/**
    @brief The supervisor call interrupt handler

    The CPU will start executing this function. Just trap here as a debug
    solution.
*/
void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{
    while( 1 )
    {
        /* Do Nothing! */
        //LED_ON();
    }
}


/**
    @brief The prefetch abort interrupt handler

    The CPU will start executing this function. Just trap here as a debug
    solution.
*/
void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{
    while( 1 )
    {
        //LED_ON();
    }
}


/**
    @brief The Data Abort interrupt handler

    The CPU will start executing this function. Just trap here as a debug
    solution.
*/
void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
    while( 1 )
    {
        //LED_ON();
    }
}


/**
    @brief The IRQ Interrupt handler

    This handler is run every time an interrupt source is triggered. It's
    up to the handler to determine the source of the interrupt and most
    importantly clear the interrupt flag so that the interrupt won't
    immediately put us back into the start of the handler again.
*/
void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
    //static int lit = 0;
    //static int ticks = 0;
    //static int seconds = 0;

 	hal_interrupt_isr();



    // Clear the ARM Timer interrupt - it's the only interrupt we have
    //   enabled, so we want don't have to work out which interrupt source
    //   caused us to interrupt
    /*RPI_GetArmTimer()->IRQClear = 1;

    ticks++;
    if( ticks > 1 )
    {
        ticks = 0;

        // Calculate the FPS once a minute /
        seconds++;
        if( seconds > 59 )
        {
            seconds = 0;
        }
    }

    // Flip the LED /
    if( lit )
    {
        LED_OFF();
    	RPI_SetGpioLo( RPI_GPIO5 );
        lit = 0;
    }
    else
    {
        LED_ON();
    	RPI_SetGpioHi( RPI_GPIO5 );
        lit = 1;
    }*/
}


/**
    @brief The FIQ Interrupt Handler

    The FIQ handler can only be allocated to one interrupt source. The FIQ has
    a full CPU shadow register set. Upon entry to this function the CPU
    switches to the shadow register set so that there is no need to save
    registers before using them in the interrupt.

    In C you can't see the difference between the IRQ and the FIQ interrupt
    handlers except for the FIQ knowing it's source of interrupt as there can
    only be one source, but the prologue and epilogue code is quite different.
    It's much faster on the FIQ interrupt handler.

    The prologue is the code that the compiler inserts at the start of the
    function, if you like, think of the opening curly brace of the function as
    being the prologue code. For the FIQ interrupt handler this is nearly
    empty because the CPU has switched to a fresh set of registers, there's
    nothing we need to save.

    The epilogue is the code that the compiler inserts at the end of the
    function, if you like, think of the closing curly brace of the function as
    being the epilogue code. For the FIQ interrupt handler this is nearly
    empty because the CPU has switched to a fresh set of registers and so has
    not altered the main set of registers.
*/
void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{

}








