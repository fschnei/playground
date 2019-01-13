

#include "rpi_armTimer.h"

static volatile rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;



volatile rpi_arm_timer_t * hal_armTimer_GetBase(void)
{
    return rpiArmTimer;
}

void hal_armTimer_Init(void)
{

	// Setup the system timer interrupt
	// Timer frequency = Clk/256 * 0x400
	hal_armTimer_GetBase()->Load = 0x4000;

	// Setup the ARM Timer
	hal_armTimer_GetBase()->Control =
			RPI_ARMTIMER_CTRL_23BIT |
			RPI_ARMTIMER_CTRL_ENABLE |
			RPI_ARMTIMER_CTRL_INT_ENABLE |
			RPI_ARMTIMER_CTRL_PRESCALE_256;
}


