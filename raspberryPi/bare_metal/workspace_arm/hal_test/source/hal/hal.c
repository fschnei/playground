/*
 * hal.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#include "hal.h"


void hal_Init(void)
{


	hal_gpio_SetInput( HAL_GPIO_PIN5 );
	//hal_gpio_SetPinFunction( HAL_GPIO_PIN5, HAL_GPIO_FUNC_SEL_INPUT );


	// Initialise the UART
	//hal_auxiliaries_MiniUartInit( 9600, 8 );


// debug
	hal_gpio_SetInput( HAL_GPIO_PIN14 );
    volatile hal_error_status_t test = hal_gpio_SetPullUpDown( HAL_GPIO_PIN14, HAL_GPIO_PULLMODE_UP );
    volatile hal_error_status_t tesx = hal_gpio_SetPullUpDown( HAL_GPIO_PIN14, HAL_GPIO_PULLMODE_DOWN );
    volatile hal_error_status_t tesg = hal_gpio_SetPullUpDown( HAL_GPIO_PIN14, HAL_GPIO_PULLMODE_UP );


	hal_armTimer_Init();


	hal_gpio_EnablePinInterrupt(HAL_GPIO_PIN5, HAL_GPIO_DETECT_RISING);


	hal_interrupt_Init();


	// enable interrupts
	hal_interrupt_UnblockIrq();

}









