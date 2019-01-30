

#include "rpi_armTimer.h"

static volatile hal_armTimer_regs_t * rpiArmTimer = (hal_armTimer_regs_t*)HAL_ARMTIMER_BASE;


// if possible use functions instead of direct register access
volatile hal_armTimer_regs_t * hal_armTimer_GetRegs(void)
{
    return rpiArmTimer;
}

void hal_armTimer_Init(void)
{

	// timer frequency = clk/256 * 0x400
	rpiArmTimer->Load = 0x4000;

	// setup the ARM timer and enable interrupt
	rpiArmTimer->Control =
			HAL_ARMTIMER_CTRL_WIDTHBIT23 |
			HAL_ARMTIMER_CTRL_ENABLE |
			HAL_ARMTIMER_CTRL_INTENABLE |
			HAL_ARMTIMER_CTRL_PRESCALE256;
}


void hal_armTimer_ClearIrq(void)
{
	rpiArmTimer->IrqClear = 1;
}

