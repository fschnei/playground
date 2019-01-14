#include "asm_prototypes.h"

#define HAL_RPI_PERIPHERAL_BASE     0x3F000000UL



#define GPIO_0_IRQ_NUM	49

#define HAL_INTERRUPT_BASE		( 0x3F00B200UL )
#define INTERRUPT_PENDING_BASIC	( HAL_INTERRUPT_BASE + 0x00UL )
#define INTERRUPT_PENDING_1		( HAL_INTERRUPT_BASE + 0x04UL )
#define INTERRUPT_PENDING_2		( HAL_INTERRUPT_BASE + 0x08UL )
#define INTERRUPT_ENABLE_1		( HAL_INTERRUPT_BASE + 0x10UL )
#define INTERRUPT_ENABLE_2		( HAL_INTERRUPT_BASE + 0x14UL )
#define INTERRUPT_ENABLE_BASIC	( HAL_INTERRUPT_BASE + 0x18UL )
#define INTERRUPT_DISABLE_1		( HAL_INTERRUPT_BASE + 0x1CUL )
#define INTERRUPT_DISABLE_2		( HAL_INTERRUPT_BASE + 0x20UL )

#define HAL_GPIO_BASE			( HAL_RPI_PERIPHERAL_BASE + 0x200000UL )
#define GPIO_GPFSEL_0			( HAL_GPIO_BASE + 0x00UL )
#define GPIO_GPLEVL_0			( HAL_GPIO_BASE + 0x34UL )
#define GPIO_GPLEVL_1			( HAL_GPIO_BASE + 0x38UL )
#define GPIO_GPEDS_0			( HAL_GPIO_BASE + 0x40UL )
#define GPIO_GPEDS_1			( HAL_GPIO_BASE + 0x44UL )
#define GPIO_GPREN_0			( HAL_GPIO_BASE + 0x4CUL )
#define GPIO_GPREN_1			( HAL_GPIO_BASE + 0x50UL )

#define HAL_ARMTIMER_BASE		( HAL_RPI_PERIPHERAL_BASE + 0x00B400UL )
#define HAL_ARMTIMER_LOAD		( HAL_GPIO_BASE + 0x00UL )
#define HAL_ARMTIMER_VALUE		( HAL_GPIO_BASE + 0x04UL )
#define HAL_ARMTIMER_CONTROL	( HAL_GPIO_BASE + 0x08UL )
#define HAL_ARMTIMER_CLEAR		( HAL_GPIO_BASE + 0x0CUL )
#define HAL_ARMTIMER_RAWIRQ		( HAL_GPIO_BASE + 0x10UL )
#define HAL_ARMTIMER_MASKEDIRQ	( HAL_GPIO_BASE + 0x14UL )
#define HAL_ARMTIMER_RELOAD		( HAL_GPIO_BASE + 0x18UL )

void hal_interrupt_unblock (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsie i" ::: "memory");
}

void hal_interrupt_block (void)
{
	// https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
	// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBFEIB.html
	asm volatile ("cpsid i" ::: "memory");
}






void _cstartup( unsigned int r0, unsigned int r1, unsigned int r2 )
{
	volatile unsigned long buffer_cpsr;
	volatile unsigned long buffer_lev0;
	volatile unsigned long buffer_lev1;
	volatile unsigned long buffer_REN;
	volatile unsigned long buffer_EN2;
	volatile unsigned long buffer_TimerCtrl;
	volatile unsigned long buffer_Timer_rawIrq;
	volatile unsigned long buffer_Timer_maskedIrq;

	// GPIO 5 rising edge interrupt enable

	//buffer_REN = GET32(GPIO_GPREN_0);
	buffer_REN = 0x20UL;
	PUT32(GPIO_GPREN_0, buffer_REN);
	//PUT32(GPIO_GPREN_0, 0xFFFFFFFFUL);


	//buffer_EN2 = GET32(INTERRUPT_ENABLE_2);
	buffer_EN2 = ( 1UL << ( 17 ) );
	PUT32(INTERRUPT_ENABLE_2, buffer_EN2);
	//PUT32(INTERRUPT_ENABLE_2, 0xFFFFFFFFUL);

	//PUT32(GPIO_GPREN_0, 0xFFFFFFFFUL);
	//PUT32(GPIO_GPREN_1, 0x001FFFFFUL);

	// clear all pending GPIO bits
	//PUT32(GPIO_GPEDS_0, 0xFFFFFFFFUL);
	//PUT32(GPIO_GPEDS_1, 0x001FFFFFUL);


	buffer_TimerCtrl = GET32(HAL_ARMTIMER_CONTROL);
	buffer_Timer_rawIrq = GET32(HAL_ARMTIMER_RAWIRQ);
	buffer_Timer_maskedIrq = GET32(HAL_ARMTIMER_MASKEDIRQ);
	//PUT32(INTERRUPT_ENABLE_2, buffer_EN2);


	hal_interrupt_unblock();

    while(1)
    {
    	buffer_lev0 = GET32(GPIO_GPLEVL_0);
    	//buffer_lev1 = GET32(GPIO_GPLEVL_1);
    	//hal_interrupt_block ();
    	//buffer_cpsr = hal_GetCPSR();
    	//_enable_interrupts();
    }
}







// ----------------------------------------------------------------------------------------
// CATION: function not declared as interrupt have no epilog and no porolog
//void interrupt_vector(void)
void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
	volatile unsigned long buffer;
	volatile unsigned long buffer_pending1;
	volatile unsigned long buffer_pending2;
	volatile unsigned long buffer_pendingBasic;

	buffer_pending1 = GET32 ( INTERRUPT_PENDING_1 );
	buffer_pending2 = GET32 ( INTERRUPT_PENDING_2 );
	buffer_pendingBasic = GET32 ( INTERRUPT_PENDING_BASIC );

	// clear every bit in pending 1 2 regs
	buffer = GET32 ( GPIO_GPEDS_0 );
	PUT32(GPIO_GPEDS_0, ( 1 << 5 ) );
	//PUT32(GPIO_GPEDS_0, ( 1 << 5 ) );
	//PUT32(GPIO_GPEDS_0, ( 1 << 6 ) );
	buffer = GET32 ( GPIO_GPEDS_0 );
	//PUT32(GPIO_GPEDS_0, 0xFFFFFFFFUL);
	//PUT32(GPIO_GPEDS_1, 0x001FFFFFUL);
	// clear timer bit
	//PUT32(HAL_ARMTIMER_CLEAR, 0xFFFFFFFFUL);


	buffer_pending1 = GET32 ( INTERRUPT_PENDING_1 );
	buffer_pending2 = GET32 ( INTERRUPT_PENDING_2 );
	buffer_pendingBasic = GET32 ( INTERRUPT_PENDING_BASIC );

}

