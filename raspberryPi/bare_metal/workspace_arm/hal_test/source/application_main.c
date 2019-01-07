

#include "rpi-aux.h"
#include "rpi-armtimer.h"
#include "rpi-gpio.h"
#include "rpi-interrupts.h"
#include "rpi-systimer.h"



/** Main function - we'll never return from here */
void application_main( unsigned int r0, unsigned int r1, unsigned int atags )
{
	/* Write 1 to the LED init nibble in the Function Select GPIO
	   peripheral register to enable LED pin as an output */
	RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;

	//extern void RPI_SetGpioOutput( rpi_gpio_pin_t gpio );
	RPI_SetGpioOutput( RPI_GPIO5 );
	

    /* Enable the timer interrupt IRQ */
    RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

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

    while( 1 )
    {
    	//LED_ON();
    	//RPI_SetGpioLo( RPI_GPIO5 );
    	RPI_WaitMicroSeconds( 1000000 );
    	//LED_OFF();
    	//RPI_SetGpioHi( RPI_GPIO5 );
    	RPI_WaitMicroSeconds( 1000000 );
    }
}
