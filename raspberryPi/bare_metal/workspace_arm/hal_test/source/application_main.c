
#include "hal/hal.h"




void application_main()
{
	uint8_t Buffer[2];
	volatile hal_bsc_error_t ret;
	uint8_t slaveAdr;
	slaveAdr = 0;

	hal_Init();


    while( 1 )
    {
    	/*slaveAdr++;
    	if ( slaveAdr > 0x27 )
    	{
    		slaveAdr = 0x20;
    	}
    	ret = hal_bsc_ReadTransaction_I2C1( slaveAdr, Buffer, 16 );

    	if ( ret == HAL_ERROR_NO_ERROR )
    	{
    		//slaveAdr = 0;
    		ret = 1;
    	}*/

    	Buffer[0] = 0xAA;
    	Buffer[1] = 0xAA;
    	//ret = hal_bsc_ReadTransaction_I2C1( 0x20, Buffer, 1 );
    	ret = hal_bsc_WriteTransaction_I2C1( 0x20, Buffer, sizeof(Buffer) );



    	//LED_ON();
    	//RPI_SetGpioLo( RPI_GPIO5 );
    	hal_sysTimer_WaitMicroSeconds( 10000 );
    	//LED_OFF();
    	//RPI_SetGpioHi( RPI_GPIO5 );
    	//RPI_WaitMicroSeconds( 1000000 );

    	hal_auxiliaries_MiniUartWrite( 'A' );


    }
}
