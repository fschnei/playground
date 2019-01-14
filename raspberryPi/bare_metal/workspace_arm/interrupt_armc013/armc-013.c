/*
    Part of the Raspberry-Pi Bare Metal Tutorials
    https://www.valvers.com/rpi/bare-metal/
    Copyright (c) 2013-2018, Brian Sidebotham

    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.

*/
/*
    Interrupts example, show how to use the interrupt controller and to load
    the vector table at runtime.
*/

#include <string.h>
#include <stdlib.h>

#include "rpi-gpio.h"
#include "rpi-armtimer.h"
#include "rpi-interrupts.h"
#include "asm_prototypes.h"


#define HAL_INTERRUPT_BASE		( 0x3F00B200UL )
#define INTERRUPT_PENDING_1		( HAL_INTERRUPT_BASE + 0x04UL )
#define INTERRUPT_PENDING_2		( HAL_INTERRUPT_BASE + 0x08UL )
#define INTERRUPT_ENABLE_1		( HAL_INTERRUPT_BASE + 0x10UL )
#define INTERRUPT_ENABLE_2		( HAL_INTERRUPT_BASE + 0x14UL )
#define INTERRUPT_ENABLE_BASIC	( HAL_INTERRUPT_BASE + 0x18UL )
#define INTERRUPT_DISABLE_1		( HAL_INTERRUPT_BASE + 0x1CUL )
#define INTERRUPT_DISABLE_2		( HAL_INTERRUPT_BASE + 0x20UL )


extern void _enable_interrupts(void);

/** Main function - we'll never return from here */
void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags )
{
    /* Write 1 to the LED init nibble in the Function Select GPIO
       peripheral register to enable LED pin as an output */
    RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;

    /* Enable the timer interrupt IRQ */
    //RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

    PUT32(INTERRUPT_ENABLE_BASIC, RPI_BASIC_ARM_TIMER_IRQ);

    /* Setup the system timer interrupt */
    /* Timer frequency = Clk/256 * 0x400 */
    RPI_GetArmTimer()->Load = 0x400;

    /* Setup the ARM Timer */
    RPI_GetArmTimer()->Control =
            RPI_ARMTIMER_CTRL_23BIT |
            RPI_ARMTIMER_CTRL_ENABLE |
            RPI_ARMTIMER_CTRL_INT_ENABLE |
            RPI_ARMTIMER_CTRL_PRESCALE_256;

    /* Enable interrupts! */
    _enable_interrupts();

    /* Never exit as there is no OS to exit to! */
    while(1)
    {

    }
}
