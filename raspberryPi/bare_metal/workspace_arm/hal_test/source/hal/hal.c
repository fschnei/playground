/*
 * hal.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#include "hal.h"

#include "interrupt/rpi_interrupts.h"
#include "../asm_prototypes.h"

void hal_init(void)
{
	volatile unsigned long buffer_cpsr;

	/* Write 1 to the LED init nibble in the Function Select GPIO
	   peripheral register to enable LED pin as an output */
	//RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;

	buffer_cpsr = hal_GetCPSR();

	//extern void RPI_SetGpioOutput( rpi_gpio_pin_t gpio );
	hal_gpio_SetInput( HAL_GPIO_PIN5 );
	//hal_gpio_SetPinFunction( HAL_GPIO_PIN5, HAL_GPIO_FUNC_SEL_INPUT );

	// Initialise the UART
	RPI_AuxMiniUartInit( 9600, 8 );

	// Enable the timer interrupt IRQ ... this is done in hal_interrupt_init
	//RPI_GetIrqController()->EnableBasicIRQs = RPI_BASIC_ARM_TIMER_IRQ;

	hal_armTimer_Init();

	/*// Setup the system timer interrupt
	// Timer frequency = Clk/256 * 0x400
	hal_armTimer_GetBase()->Load = 0x4000;

	// Setup the ARM Timer
	hal_armTimer_GetBase()->Control =
			RPI_ARMTIMER_CTRL_23BIT |
			RPI_ARMTIMER_CTRL_ENABLE |
			RPI_ARMTIMER_CTRL_INT_ENABLE |
			RPI_ARMTIMER_CTRL_PRESCALE_256;*/

	buffer_cpsr = hal_GetCPSR();

	//EnableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
	hal_gpio_EnablePinInterrupt(HAL_GPIO_PIN5, HAL_GPIO_DETECT_RISING);

	buffer_cpsr = hal_GetCPSR();

	hal_interrupt_init();

	buffer_cpsr = hal_GetCPSR();


	//PUT32(GPIO_GPREN_0, 0x20UL);
	//PUT32(INTERRUPT_ENABLE_2, ( 1UL << ( 17 ) ));

	buffer_cpsr = hal_GetCPSR();


	// Enable interrupts!
	hal_interrupt_unblock();

	buffer_cpsr = hal_GetCPSR();
}









