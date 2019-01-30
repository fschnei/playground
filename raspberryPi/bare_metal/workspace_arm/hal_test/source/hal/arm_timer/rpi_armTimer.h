
#ifndef RPI_ARMTIMER_H
#define RPI_ARMTIMER_H

#include "../rpi_base.h"

// see section 14
#define HAL_ARMTIMER_BASE               ( HAL_RPI_PERIPHERAL_BASE + 0xB400UL )

// 0: 16-bit counter
// 1: 23-bit counter
#define HAL_ARMTIMER_CTRL_WIDTHBIT16	( 0 << 1 )
#define HAL_ARMTIMER_CTRL_WIDTHBIT23	( 1 << 1 )

#define HAL_ARMTIMER_CTRL_PRESCALE1		( 0 << 2 )
#define HAL_ARMTIMER_CTRL_PRESCALE16	( 1 << 2 )
#define HAL_ARMTIMER_CTRL_PRESCALE256	( 2 << 2 )

#define HAL_ARMTIMER_CTRL_INTDISABLE	( 0 << 5 )
#define HAL_ARMTIMER_CTRL_INTENABLE		( 1 << 5 )

#define HAL_ARMTIMER_CTRL_DISABLE		( 0 << 7 )
#define HAL_ARMTIMER_CTRL_ENABLE		( 1 << 7 )


typedef struct{
	hal_reg_rw_t 	Load;
	hal_reg_r_t 	Value;
    hal_reg_rw_t 	Control;
    hal_reg_w_t 	IrqClear;
    hal_reg_r_t 	RAWIRQ;
    hal_reg_r_t 	MaskedIrq;
    hal_reg_rw_t 	Reload;
    hal_reg_rw_t 	PreDivider;
    hal_reg_r_t 	FreeRunningCounter;
} hal_armTimer_regs_t;


volatile hal_armTimer_regs_t * hal_armTimer_GetRegs(void);

void hal_armTimer_Init(void);

void hal_armTimer_ClearIrq(void);



#endif
