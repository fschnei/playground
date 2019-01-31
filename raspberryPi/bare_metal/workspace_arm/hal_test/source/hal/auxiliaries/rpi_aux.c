
#include "../gpio/rpi_gpio.h"
#include "rpi_aux.h"
#include "../rpi_base.h"

static hal_auxiliaries_regs_t * hal_auxillary_regs = (hal_auxiliaries_regs_t *)HAL_AUXILIARIES_BASE;


hal_auxiliaries_regs_t * hal_auxiliaries_GetRegs( void )
{
    return hal_auxillary_regs;
}


void hal_auxiliaries_MiniUartInit( int baud, int bits )
{
    volatile int i;

    // set enable bit to enable mini uart (also register rw access)
    hal_auxillary_regs->Enable = AUX_ENA_MINIUART;

    /* Disable interrupts for now */
    /* auxillary->IRQ &= ~AUX_IRQ_MU; */

    hal_auxillary_regs->MiniUart_IER = 0;

    // disable rx, tx, flow control, ...
    hal_auxillary_regs->MiniUart_CNTL = 0;

    // activate 8 or 7 bit mode
    if( bits == 8 )
    	hal_auxillary_regs->MiniUart_LCR = AUX_MULCR_8BIT_MODE;
    else
    	hal_auxillary_regs->MiniUart_LCR = 0;

    // set rts line low
    hal_auxillary_regs->MiniUart_MCR = 0;

    // Disable all interrupts from MU and clear the fifos
    hal_auxillary_regs->MiniUart_IER = 0;

    //
    hal_auxillary_regs->MiniUart_IIR = 0xC6;

    // set baud rate
    hal_auxillary_regs->MiniUart_BAUD = ( SYS_FREQ / ( 8 * baud ) ) - 1;

    // set rx and tx pin to corresponding alternative function
    hal_gpio_SetPinFunction( HAL_GPIO_PIN14, HAL_GPIO_FUNC_SEL_ALT5 );
    hal_gpio_SetPinFunction( HAL_GPIO_PIN15, HAL_GPIO_FUNC_SEL_ALT5 );

    //
    hal_gpio_GetRegs()->GPPUD[0] = 0;
    for( i=0; i<150; i++ ) { }
    hal_gpio_GetRegs()->GPPUDCLK[0] = ( 1 << 14 );
    for( i=0; i<150; i++ ) { }
    hal_gpio_GetRegs()->GPPUDCLK[0] = 0;

    // enable mini uart tx
    hal_auxillary_regs->MiniUart_CNTL = AUX_MUCNTL_TX_ENABLE;
}


void hal_auxiliaries_MiniUartWrite( char c )
{
    while( ( hal_auxillary_regs->MiniUart_LSR & AUX_MULSR_TX_EMPTY ) == 0 ) { }

    hal_auxillary_regs->MiniUart_Io = c;
}




