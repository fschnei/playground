
#include "rpi_gpio.h"


static volatile bcm_gpio_regs_t * const hal_gpio_regs = (bcm_gpio_regs_t *) (HAL_GPIO_BASE);


// provide pinter to gpio registers via function
// -> read only access to hal_gpio_regs
// -> no change of hal_gpio_regs by mistake
volatile bcm_gpio_regs_t* const hal_gpio_getBase(void)
{
    return hal_gpio_regs;
}


void hal_gpio_SetPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func )
{
    rpi_reg_rw_t fsel_copy = hal_gpio_regs->GPFSEL[gpio/10];
    fsel_copy &= ~( HAL_GPIO_FUNC_SEL_CLR_MASK << ( ( gpio % 10 ) * 3 ) );
    fsel_copy |= (func << ( ( gpio % 10 ) * 3 ) );
    hal_gpio_regs->GPFSEL[gpio/10] = fsel_copy;
}


void hal_gpio_SetOutput( rpi_gpio_pin_t gpio )
{
	hal_gpio_SetPinFunction( gpio, HAL_GPIO_FUNC_SEL_OUTPUT );
}


void hal_gpio_SetInput( rpi_gpio_pin_t gpio )
{
	hal_gpio_SetPinFunction( gpio, HAL_GPIO_FUNC_SEL_INPUT );
}


hal_gpio_level_t hal_gpio_GetValue( rpi_gpio_pin_t gpio )
{
	hal_gpio_level_t result = HAL_GPIO_LVL_UNKNOWN;

    switch( gpio / 32 )
    {
        case 0:
            result = hal_gpio_regs->GPLEV[0] & ( 1 << gpio );
            break;

        case 1:
            result = hal_gpio_regs->GPLEV[1] & ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }

    if( result != HAL_GPIO_LVL_UNKNOWN )
    {
        if( result )
            result = HAL_GPIO_LVL_HI;
    }

    return result;
}


void hal_gpio_Toggle( rpi_gpio_pin_t gpio )
{
    if( hal_gpio_GetValue( gpio ) )
    	hal_gpio_SetLo( gpio );
    else
    	hal_gpio_SetHi( gpio );
}


void hal_gpio_SetHi( rpi_gpio_pin_t gpio )
{
    switch( gpio / 32 )
    {
        case 0:
        	hal_gpio_regs->GPSET[0] = ( 1 << gpio );
            break;

        case 1:
        	hal_gpio_regs->GPSET[1] = ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }
}


void hal_gpio_SetLo( rpi_gpio_pin_t gpio )
{
    switch( gpio / 32 )
    {
        case 0:
        	hal_gpio_regs->GPCLR[0] = ( 1 << gpio );
            break;

        case 1:
        	hal_gpio_regs->GPCLR[1] = ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }
}


void hal_gpio_SetValue( rpi_gpio_pin_t gpio, hal_gpio_level_t value )
{
    if( ( value == HAL_GPIO_LVL_LO ) || ( value == HAL_GPIO_LVL_OFF ) )
    	hal_gpio_SetLo( gpio );
    else if( ( value == HAL_GPIO_LVL_HI ) || ( value == HAL_GPIO_LVL_ON ) )
    	hal_gpio_SetLo( gpio );
}







void hal_gpio_EnablePinInterrupt(unsigned int pinNum, enum DETECT_TYPE type)
{
	unsigned long mask=(1<<pinNum);
	unsigned long offset=pinNum/32;

	switch(type) {
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[offset]|=mask;
		break;
	case HAL_GPIO_DETECT_NONE:
		break;
	}
}

void hal_gpio_DisablePinInterrupt(unsigned int pinNum, enum DETECT_TYPE type)
{
	unsigned long mask=~(1<<(pinNum%32));
	unsigned long offset=pinNum/32;

	switch(type) {
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[offset]&=mask;
		break;
	case HAL_GPIO_DETECT_NONE:
		break;
	}
}

void hal_gpio_ClearInterrupt(unsigned int pinNum)
{
	unsigned long mask=(1<<(pinNum%32));
	unsigned long offset=pinNum/32;

	hal_gpio_regs->GPEDS[offset]=mask;
}



