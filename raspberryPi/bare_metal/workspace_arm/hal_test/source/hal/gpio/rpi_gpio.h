
#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include "../rpi_base.h"

// gpio access works via memory mapped io
// this is the base address for gpio pins
#define HAL_GPIO_BASE       ( HAL_RPI_PERIPHERAL_BASE + 0x200000UL )

#if defined( RPIBPLUS ) || defined( RPI2 )
    #define LED_GPFSEL      GPFSEL4
    #define LED_GPFBIT      21
    #define LED_GPSET       GPSET1
    #define LED_GPCLR       GPCLR1
    #define LED_GPIO_BIT    15
    //#define LED_ON()        do { RPI_GetGpio()->LED_GPCLR = ( 1 << LED_GPIO_BIT ); } while( 0 )
    //#define LED_OFF()       do { RPI_GetGpio()->LED_GPSET = ( 1 << LED_GPIO_BIT ); } while( 0 )
#else
    #define LED_GPFSEL      GPFSEL1
    #define LED_GPFBIT      18
    #define LED_GPSET       GPSET0
    #define LED_GPCLR       GPCLR0
    #define LED_GPIO_BIT    16
    //#define LED_ON()        do { RPI_GetGpio()->LED_GPSET = ( 1 << LED_GPIO_BIT ); } while( 0 )
    //#define LED_OFF()       do { RPI_GetGpio()->LED_GPCLR = ( 1 << LED_GPIO_BIT ); } while( 0 )
#endif

typedef enum {
	HAL_GPIO_FUNC_SEL_INPUT = 0,
	HAL_GPIO_FUNC_SEL_OUTPUT,
	HAL_GPIO_FUNC_SEL_ALT5,
	HAL_GPIO_FUNC_SEL_ALT4,
	HAL_GPIO_FUNC_SEL_ALT0,
	HAL_GPIO_FUNC_SEL_ALT1,
	HAL_GPIO_FUNC_SEL_ALT2,
	HAL_GPIO_FUNC_SEL_ALT3,
    } rpi_gpio_alt_function_t;

// A mask to be able to clear the bits in the register before setting the value we require
// see page 92 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
#define HAL_GPIO_FUNC_SEL_CLR_MASK     (7)


typedef enum {
    HAL_GPIO_PIN0 = 0,
	HAL_GPIO_PIN1,
    HAL_GPIO_PIN2,
    HAL_GPIO_PIN3,
    HAL_GPIO_PIN4,
    HAL_GPIO_PIN5,
    HAL_GPIO_PIN6,
    HAL_GPIO_PIN7,
    HAL_GPIO_PIN8,
    HAL_GPIO_PIN9,
    HAL_GPIO_PIN10 = 10,
    HAL_GPIO_PIN11,
    HAL_GPIO_PIN12,
    HAL_GPIO_PIN13,
    HAL_GPIO_PIN14,
    HAL_GPIO_PIN15,
    HAL_GPIO_PIN16,
    HAL_GPIO_PIN17,
    HAL_GPIO_PIN18,
    HAL_GPIO_PIN19,
    HAL_GPIO_PIN20 = 20,
    HAL_GPIO_PIN21,
    HAL_GPIO_PIN22,
    HAL_GPIO_PIN23,
    HAL_GPIO_PIN24,
    HAL_GPIO_PIN25,
    HAL_GPIO_PIN26,
    HAL_GPIO_PIN27,
    HAL_GPIO_PIN28,
    HAL_GPIO_PIN29,
    HAL_GPIO_PIN30 = 30,
    HAL_GPIO_PIN31,
    HAL_GPIO_PIN32,
    HAL_GPIO_PIN33,
    HAL_GPIO_PIN34,
    HAL_GPIO_PIN35,
    HAL_GPIO_PIN36,
    HAL_GPIO_PIN37,
    HAL_GPIO_PIN38,
    HAL_GPIO_PIN39,
    HAL_GPIO_PIN40 = 40,
    HAL_GPIO_PIN41,
    HAL_GPIO_PIN42,
    HAL_GPIO_PIN43,
    HAL_GPIO_PIN44,
    HAL_GPIO_PIN45,
    HAL_GPIO_PIN46,
    HAL_GPIO_PIN47,
    HAL_GPIO_PIN48,
    HAL_GPIO_PIN49,
    HAL_GPIO_PIN50 = 50,
    HAL_GPIO_PIN51,
    HAL_GPIO_PIN52,
    HAL_GPIO_PIN53,
    } rpi_gpio_pin_t;


// see page 90 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
typedef struct {
	hal_reg_rw_t	GPFSEL[6];		// Function Select
	hal_reg_r_t	Reserved_1;
	hal_reg_w_t	GPSET[2];		// Pin Output Set
	hal_reg_r_t	Reserved_2;
	hal_reg_w_t	GPCLR[2];		// Pin Output Clear
	hal_reg_r_t	Reserved_3;
	hal_reg_w_t	GPLEV[2];		// Pin Level
	hal_reg_r_t	Reserved_4;
	hal_reg_w_t	GPEDS[2];		// Pin Event Detect Status
	hal_reg_r_t	Reserved_5;
	hal_reg_w_t	GPREN[2];		// Pin Rising Edge Detect Enable
	hal_reg_r_t	Reserved_6;
	hal_reg_w_t	GPFEN[2];		// Pin Falling Edge Detect Enable
	hal_reg_r_t	Reserved_7;
	hal_reg_w_t	GPHEN[2];		// Pin High Detect Enable
	hal_reg_r_t	Reserved_8;
	hal_reg_w_t	GPLEN[2];		// Pin Low Detect Enable
	hal_reg_r_t	Reserved_9;
	hal_reg_w_t	GPAREN[2];		// Pin Async. Rising Edge Detect
	hal_reg_r_t	Reserved_A;
	hal_reg_w_t	GPAFEN[2];		// Pin Async. Falling Edge Detect
	hal_reg_r_t	Reserved_B;
	hal_reg_w_t	GPPUD[1];		// Pin Pull-up/down Enable
	hal_reg_w_t	GPPUDCLK[2];	// Pin Pull-up/down Enable Clock
	hal_reg_r_t	Reserved_C;
} hal_gpio_regs_t;


typedef enum {
	HAL_GPIO_LVL_LO = 0,
	HAL_GPIO_LVL_HI,
	HAL_GPIO_LVL_ON,	// TODO: is this necessary ?
	HAL_GPIO_LVL_OFF,	// TODO: is this necessary ?
	HAL_GPIO_LVL_UNKNOWN,
} hal_gpio_level_t;

typedef enum {
	HAL_GPIO_DETECT_NONE,
	HAL_GPIO_DETECT_RISING,
	HAL_GPIO_DETECT_FALLING,
	HAL_GPIO_DETECT_HIGH,
	HAL_GPIO_DETECT_LOW,
	HAL_GPIO_DETECT_RISING_ASYNC,
	HAL_GPIO_DETECT_FALLING_ASYNC
} hal_gpio_detectMode_t;



volatile hal_gpio_regs_t * const hal_gpio_GetRegs(void);

void hal_gpio_SetPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func );
void hal_gpio_SetOutput( rpi_gpio_pin_t gpio );
void hal_gpio_SetInput( rpi_gpio_pin_t gpio );

hal_gpio_level_t hal_gpio_GetValue( rpi_gpio_pin_t gpio );

void hal_gpio_SetHi( rpi_gpio_pin_t gpio );
void hal_gpio_SetLo( rpi_gpio_pin_t gpio );
void hal_gpio_SetValue( rpi_gpio_pin_t gpio, hal_gpio_level_t value );
void hal_gpio_Toggle( rpi_gpio_pin_t gpio );

// see p. 96 ff. https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
/* Interrupt related functions */
void hal_gpio_EnablePinInterrupt	(unsigned int pinNum, hal_gpio_detectMode_t detectMode);
void hal_gpio_DisablePinInterrupt	(unsigned int pinNum, hal_gpio_detectMode_t detectMode);
void hal_gpio_ClearInterrupt		(unsigned int pinNum);


#endif
