/*
 * hal.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#include "hal.h"


void hal_init(void)
{
	/* Write 1 to the LED init nibble in the Function Select GPIO
	   peripheral register to enable LED pin as an output */
	//RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;

	//extern void RPI_SetGpioOutput( rpi_gpio_pin_t gpio );
	hal_gpio_SetInput( HAL_GPIO_PIN5 );

	// Initialise the UART
	RPI_AuxMiniUartInit( 9600, 8 );

	irq_init();

	// Enable the timer interrupt IRQ
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

	// Setup the system timer interrupt
	// Timer frequency = Clk/256 * 0x400
	RPI_GetArmTimer()->Load = 0x4000;

	// Setup the ARM Timer
	RPI_GetArmTimer()->Control =
			RPI_ARMTIMER_CTRL_23BIT |
			RPI_ARMTIMER_CTRL_ENABLE |
			RPI_ARMTIMER_CTRL_INT_ENABLE |
			RPI_ARMTIMER_CTRL_PRESCALE_256;


	//EnableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
	hal_gpio_EnablePinInterrupt(HAL_GPIO_PIN5, HAL_GPIO_DETECT_RISING);


	// Enable interrupts!
	_enable_interrupts();
}









