
#include "rpi-gpio.h"

//static rpi_gpio_t* rpiGpio = (rpi_gpio_t*)RPI_GPIO_BASE;

// ( PERIPHERAL_BASE + 0x200000UL )
//volatile BCM2835_GPIO_REGS * const pRegs = (BCM2835_GPIO_REGS *) (0x20200000);
volatile BCM2835_GPIO_REGS * const pRegs_ = (BCM2835_GPIO_REGS *) (0x3F200000);



volatile BCM2835_GPIO_REGS* const RPI_GetGpio(void)
{
    return pRegs_;
}


void RPI_SetGpioPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func )
{
    rpi_reg_rw_t* fsel_reg = &((rpi_reg_rw_t*)pRegs_)[ gpio / 10 ];
    rpi_reg_rw_t fsel_copy = *fsel_reg;
    fsel_copy &= ~( FS_MASK << ( ( gpio % 10 ) * 3 ) );
    fsel_copy |= (func << ( ( gpio % 10 ) * 3 ) );
    *fsel_reg = fsel_copy;
}


void RPI_SetGpioOutput( rpi_gpio_pin_t gpio )
{
    RPI_SetGpioPinFunction( gpio, FS_OUTPUT );
}


void RPI_SetGpioInput( rpi_gpio_pin_t gpio )
{
    RPI_SetGpioPinFunction( gpio, FS_INPUT );
}


rpi_gpio_value_t RPI_GetGpioValue( rpi_gpio_pin_t gpio )
{
    rpi_gpio_value_t result = RPI_IO_UNKNOWN;

    switch( gpio / 32 )
    {
        case 0:
            result = pRegs_->GPLEV[0] & ( 1 << gpio );
            break;

        case 1:
            result = pRegs_->GPLEV[1] & ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }

    if( result != RPI_IO_UNKNOWN )
    {
        if( result )
            result = RPI_IO_HI;
    }

    return result;
}


void RPI_ToggleGpio( rpi_gpio_pin_t gpio )
{
    if( RPI_GetGpioValue( gpio ) )
        RPI_SetGpioLo( gpio );
    else
        RPI_SetGpioHi( gpio );
}


void RPI_SetGpioHi( rpi_gpio_pin_t gpio )
{
    switch( gpio / 32 )
    {
        case 0:
        	pRegs_->GPSET[0] = ( 1 << gpio );
            break;

        case 1:
        	pRegs_->GPSET[1] = ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }
}


void RPI_SetGpioLo( rpi_gpio_pin_t gpio )
{
    switch( gpio / 32 )
    {
        case 0:
        	pRegs_->GPCLR[0] = ( 1 << gpio );
            break;

        case 1:
        	pRegs_->GPCLR[1] = ( 1 << ( gpio - 32 ) );
            break;

        default:
            break;
    }
}


void RPI_SetGpioValue( rpi_gpio_pin_t gpio, rpi_gpio_value_t value )
{
    if( ( value == RPI_IO_LO ) || ( value == RPI_IO_OFF ) )
        RPI_SetGpioLo( gpio );
    else if( ( value == RPI_IO_HI ) || ( value == RPI_IO_ON ) )
        RPI_SetGpioHi( gpio );
}







void EnableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
{
	unsigned long mask=(1<<pinNum);
	unsigned long offset=pinNum/32;

	switch(type) {
	case DETECT_RISING:
		pRegs_->GPREN[offset]|=mask;
		break;
	case DETECT_FALLING:
		pRegs_->GPFEN[offset]|=mask;
		break;
	case DETECT_HIGH:
		pRegs_->GPHEN[offset]|=mask;
		break;
	case DETECT_LOW:
		pRegs_->GPLEN[offset]|=mask;
		break;
	case DETECT_RISING_ASYNC:
		pRegs_->GPAREN[offset]|=mask;
		break;
	case DETECT_FALLING_ASYNC:
		pRegs_->GPAFEN[offset]|=mask;
		break;
	case DETECT_NONE:
		break;
	}
}

void DisableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
{
	unsigned long mask=~(1<<(pinNum%32));
	unsigned long offset=pinNum/32;

	switch(type) {
	case DETECT_RISING:
		pRegs_->GPREN[offset]&=mask;
		break;
	case DETECT_FALLING:
		pRegs_->GPFEN[offset]&=mask;
		break;
	case DETECT_HIGH:
		pRegs_->GPHEN[offset]&=mask;
		break;
	case DETECT_LOW:
		pRegs_->GPLEN[offset]&=mask;
		break;
	case DETECT_RISING_ASYNC:
		pRegs_->GPAREN[offset]&=mask;
		break;
	case DETECT_FALLING_ASYNC:
		pRegs_->GPAFEN[offset]&=mask;
		break;
	case DETECT_NONE:
		break;
	}
}

void ClearGpioInterrupt(unsigned int pinNum)
{
	unsigned long mask=(1<<(pinNum%32));
	unsigned long offset=pinNum/32;

	pRegs_->GPEDS[offset]=mask;
}



