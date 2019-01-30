
#include "rpi_sysTimer.h"



static volatile hal_sysTimer_regs_t * const hal_sysTimer_regs = (hal_sysTimer_regs_t * )HAL_SYSTIMER_BASE;


volatile hal_sysTimer_regs_t * hal_sysTimer_GetRegs(void)
{
    return hal_sysTimer_regs;
}

void hal_sysTimer_WaitMicroSeconds( hal_base_t us )
{
	hal_v_base_t ts = hal_sysTimer_regs->counter_lo;

    while( ( hal_sysTimer_regs->counter_lo - ts ) < us )
    {
        // nop
    }
}
