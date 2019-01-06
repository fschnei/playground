/*
 * main.c
 *
 *  Created on: 23.01.2018
 *      Author: fasch019
 */


// this file demonstrates the functionality of the hal


#include "hal.h"
#include "rpi2.h"



void main()
{

    signed int index;
	volatile int i;

    
	hal_init_system();
	/*// Pegel von GPIO5 auf Null setzen
	*GPCLR0 = (1<<5);
	// GPIO 5 als Ausgang schalten
	*GPFSEL0 &= ~(GPIOFSEL_CLR << FSEL5);
	*GPFSEL0 |=	GPIOFSEL_OUT << FSEL5;
*/
//
//	while (1)
//	{
//		// GPIO umschalten
//		if ( /*hal_getButtonState(0)*/1 == 1) //*GPLEV0 & (1<<5) )
//		{
//			*GPCLR0 = (1<<5);
//			*GPCLR0 = (1<<6);
//			*GPCLR0 = (1<<7);
//			*GPCLR0 = (1<<8);
//			*GPCLR0 = (1<<9);
//			*GPCLR0 = (1<<10);
//			*GPCLR0 = (1<<11);
//			*GPCLR0 = (1<<12);
//		}
//		else
//		{
//	        /*hal_switchLED(0, 1);
//	        hal_switchLED(1, 1);
//	        hal_switchLED(2, 1);
//	        hal_switchLED(3, 1);
//	        hal_switchLED(4, 1);
//	        hal_switchLED(5, 1);
//	        hal_switchLED(6, 1);
//	        hal_switchLED(7, 1);*/
//			*GPSET0 = (1<<5);
//			*GPSET0 = (1<<6);
//			*GPSET0 = (1<<7);
//			*GPSET0 = (1<<8);
//			*GPSET0 = (1<<9);
//			*GPSET0 = (1<<10);
//			*GPSET0 = (1<<11);
//			*GPSET0 = (1<<12);
//		}
//
////		// Warteschleife
////		i=1000;
////		while (i>0)
////			hal_wait_1ms();
//	}

    while(1)
    {
    	if (hal_getButtonState(0) == 1)
    	{
			hal_switchLED(4, 1);
        }
    	if (hal_getButtonState(1) == 1)
    	{
			hal_switchLED(5, 1);
    	}
    	if (hal_getButtonState(2) == 1)
    	{
			hal_switchLED(6, 1);
    	}
    	if (hal_getButtonState(3) == 1)
    	{
			hal_switchLED(7, 1);
    	}
    	if (hal_getSwitchState(0) == 1)
    	{
			hal_switchLED(0, 1);
    	}
    	if (hal_getSwitchState(1) == 1)
    	{
			hal_switchLED(1, 1);
    	}
    	if (hal_getSwitchState(2) == 1)
    	{
			hal_switchLED(2, 1);
    	}
    	if (hal_getSwitchState(3) == 1)
    	{
			hal_switchLED(3, 1);
    	}

//        hal_switchLED(0, 1);
//        hal_switchLED(1, 1);
//        hal_switchLED(2, 1);
//        hal_switchLED(3, 1);
//        hal_switchLED(4, 1);
//        hal_switchLED(5, 1);
//        hal_switchLED(6, 1);
//        hal_switchLED(7, 1);


		for(index = 0; index < 1000; index++)
			hal_wait_1ms();

		hal_switchLED(0, 0);
		hal_switchLED(1, 0);
		hal_switchLED(2, 0);
		hal_switchLED(3, 0);
		hal_switchLED(4, 0);
		hal_switchLED(5, 0);
		hal_switchLED(6, 0);
		hal_switchLED(7, 0);

		for(index = 0; index < 1000; index++)
			hal_wait_1ms();
    }

}
