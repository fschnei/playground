
#include "hal/hal.h"




void application_main()
{
	hal_Init();


    while( 1 )
    {
    	//LED_ON();
    	//RPI_SetGpioLo( RPI_GPIO5 );
    	hal_sysTimer_WaitMicroSeconds( 1000000 );
    	//LED_OFF();
    	//RPI_SetGpioHi( RPI_GPIO5 );
    	//RPI_WaitMicroSeconds( 1000000 );

    	RPI_AuxMiniUartWrite( 'A' );
    }
}
