

/*#include "hal/arm_timer/rpi-armtimer.h"
#include "hal/auxiliaries/rpi-aux.h"
#include "hal/gpio/rpi-gpio.h"
#include "hal/interrupt/rpi-interrupts.h"
#include "hal/interrupt/rpi_interrupts.h"
#include "hal/sys_timer/rpi-systimer.h"*/

#include "hal/hal.h"

#include "asm_prototypes.h"




/** Main function - we'll never return from here */
void application_main( unsigned int r0, unsigned int r1, unsigned int atags )
{
	hal_init();


    while( 1 )
    {
    	//LED_ON();
    	//RPI_SetGpioLo( RPI_GPIO5 );
    	RPI_WaitMicroSeconds( 1000000 );
    	//LED_OFF();
    	//RPI_SetGpioHi( RPI_GPIO5 );
    	//RPI_WaitMicroSeconds( 1000000 );

    	RPI_AuxMiniUartWrite( 'A' );
    }
}
