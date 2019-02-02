
#include "rpi_gpio.h"


static volatile hal_gpio_regs_t * const hal_gpio_regs = (hal_gpio_regs_t *) (HAL_GPIO_BASE);


// provide pinter to gpio registers via function
// -> read only access to hal_gpio_regs
// -> no change of hal_gpio_regs by mistake
volatile hal_gpio_regs_t * const hal_gpio_GetRegs(void)
{
    return hal_gpio_regs;
}


hal_error_status_t hal_gpio_SetPullUpDown( rpi_gpio_pin_t gpio, hal_gpio_pullupdown_t UpDown )
{
	volatile hal_base_t i;
    hal_base_t index = gpio / 32;
	hal_base_t mask = ( 1 << ( gpio - ( 32 * index ) ) );

	if( gpio >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

    // https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
	// page 100
	if( UpDown == HAL_GPIO_PULLMODE_UP )
	{
		hal_gpio_regs->GPPUD[0] = ( 1 << 1 );
	}
	else if ( UpDown == HAL_GPIO_PULLMODE_DOWN )
	{
		hal_gpio_regs->GPPUD[0] = ( 1 << 0 );
	}
	else if ( UpDown == HAL_GPIO_PULLMODE_DISABLE )
	{
		hal_gpio_regs->GPPUD[0] = 0x00;
	}
	else
	{
		// invalid updown mode
		return HAL_ERROR_GENERAL_ERROR;
	}
    for( i = 0; i < 150; i++ ) { }
    hal_gpio_regs->GPPUDCLK[index] = mask;
    for( i = 0; i < 150; i++ ) { }
    hal_gpio_regs->GPPUDCLK[index] = 0;

	return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func )
{
	hal_v_base_t Gpfsel_Buffer;
    hal_base_t index = gpio / 10;
	hal_base_t shift = ( gpio % 10 ) * 3;

	if( gpio >= HAL_GPIO_LASTENTRY || func >= HAL_GPIO_FUNC_SEL_LASTENTRY )
	{
		// invalid gpio pin or invalid alternative function
		return HAL_ERROR_GENERAL_ERROR;
	}

	// get current gpfsel register
	Gpfsel_Buffer = hal_gpio_regs->GPFSEL[index];
	// clear function select
	Gpfsel_Buffer &= ~ ( HAL_GPIO_FUNC_SEL_CLR_MASK << shift );
	// set new function
	Gpfsel_Buffer |= ( func << shift );
	// write back buffer to gpfsel register
    hal_gpio_regs->GPFSEL[index] = Gpfsel_Buffer;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetOutput( rpi_gpio_pin_t gpio )
{
	return hal_gpio_SetPinFunction( gpio, HAL_GPIO_FUNC_SEL_OUTPUT );
}


hal_error_status_t hal_gpio_SetInput( rpi_gpio_pin_t gpio )
{
	return hal_gpio_SetPinFunction( gpio, HAL_GPIO_FUNC_SEL_INPUT );
}


hal_error_status_t hal_gpio_GetValue( rpi_gpio_pin_t gpio, hal_gpio_level_t * Level )
{
	hal_gpio_level_t result;
    hal_base_t index = gpio / 32;
	hal_base_t mask = ( 1 << ( gpio - ( 32 * index ) ) );

	if( gpio >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

    result = hal_gpio_regs->GPLEV[index] & mask;

    if ( result != 0 )
    {
        *Level = HAL_GPIO_LVL_HI;
    }
    else
    {
        *Level = HAL_GPIO_LVL_LO;
    }

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_Toggle( rpi_gpio_pin_t gpio )
{
	hal_gpio_level_t Level;
	if( gpio >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	if ( hal_gpio_GetValue( gpio, &Level ) != HAL_ERROR_NO_ERROR )
	{
		return HAL_ERROR_GENERAL_ERROR;
	}

    if( Level == HAL_GPIO_LVL_HI )
    {
    	return hal_gpio_SetLo( gpio );
    }
    else
    {
    	return hal_gpio_SetHi( gpio );
    }
}


hal_error_status_t hal_gpio_SetHi( rpi_gpio_pin_t gpio )
{
    hal_base_t index = gpio / 32;
	hal_base_t mask = ( 1 << ( gpio - ( 32 * index ) ) );

	if( gpio >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPSET[index] = mask;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetLo( rpi_gpio_pin_t gpio )
{
    hal_base_t index = gpio / 32;
	hal_base_t mask = ( 1 << ( gpio - ( 32 * index ) ) );

	if( gpio >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPCLR[index] = mask;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetValue( rpi_gpio_pin_t gpio, hal_gpio_level_t value )
{
	if( gpio >= HAL_GPIO_LASTENTRY || value >= HAL_GPIO_LVL_LASTENTRY )
	{
		// invalid gpio pin or invalid value
		return HAL_ERROR_GENERAL_ERROR;
	}
    if( value == HAL_GPIO_LVL_LO )
    {
    	return hal_gpio_SetLo( gpio );
    }
    else if( value == HAL_GPIO_LVL_HI )
    {
    	return hal_gpio_SetHi( gpio );
    }
	return HAL_ERROR_GENERAL_ERROR;
}







hal_error_status_t hal_gpio_EnablePinInterrupt(rpi_gpio_pin_t pinNum, hal_gpio_detectMode_t detectMode)
{
	hal_base_t index = pinNum / 32;
	hal_base_t mask = ( 1 << ( pinNum - ( 32 * index ) ) );

	if( pinNum >= HAL_GPIO_LASTENTRY || detectMode >= HAL_GPIO_DETECT_LASTENTRY )
	{
		// invalid gpio pin or invalid detect mode
		return HAL_ERROR_GENERAL_ERROR;
	}

	switch( detectMode )
	{
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_ALL:
		// set all detect register
		hal_gpio_regs->GPREN[index] |= mask;
		hal_gpio_regs->GPFEN[index] |= mask;
		hal_gpio_regs->GPHEN[index] |= mask;
		hal_gpio_regs->GPLEN[index] |= mask;
		hal_gpio_regs->GPAREN[index] |= mask;
		hal_gpio_regs->GPAFEN[index] |= mask;
		break;
	}
	return HAL_ERROR_NO_ERROR;
}

hal_error_status_t hal_gpio_DisablePinInterrupt(rpi_gpio_pin_t pinNum, hal_gpio_detectMode_t detectMode)
{
	hal_base_t index = pinNum / 32;
	hal_base_t mask = ~ ( 1 << ( pinNum - ( 32 * index ) ) );

	if( pinNum >= HAL_GPIO_LASTENTRY || detectMode >= HAL_GPIO_DETECT_LASTENTRY )
	{
		// invalid gpio pin or invalid detect mode
		return HAL_ERROR_GENERAL_ERROR;
	}

	switch( detectMode )
	{
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_ALL:
		// clear all detect register
		hal_gpio_regs->GPREN[index] &= mask;
		hal_gpio_regs->GPFEN[index] &= mask;
		hal_gpio_regs->GPHEN[index] &= mask;
		hal_gpio_regs->GPLEN[index] &= mask;
		hal_gpio_regs->GPAREN[index] &= mask;
		hal_gpio_regs->GPAFEN[index] &= mask;
		break;
	}
	return HAL_ERROR_NO_ERROR;
}

hal_error_status_t hal_gpio_ClearInterrupt(rpi_gpio_pin_t pinNum)
{
	hal_base_t index = pinNum / 32;
	hal_base_t mask = ( 1 << ( pinNum - ( 32 * index ) ) );

	if( pinNum >= HAL_GPIO_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPEDS[index] = mask;

	return HAL_ERROR_NO_ERROR;
}



