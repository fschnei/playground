/*
 * rpi_bsc.c
 *
 *  Created on: 04.02.2019
 *      Author: Fabian
 */

#include "rpi_bsc.h"
#include "../gpio/rpi_gpio.h"


#define HAL_BSC0_BASE ( HAL_RPI_PERIPHERAL_BASE + 0x205000UL )
#define HAL_BSC1_BASE ( HAL_RPI_PERIPHERAL_BASE + 0x804000UL )
#define HAL_BSC2_BASE ( HAL_RPI_PERIPHERAL_BASE + 0x805000UL )

// control register
#define HAL_BSC_CONTROL_READ				( 1 << 0 )		// 1 = read transfer, 0 = write transfer
#define HAL_BSC_CONTROL_CLEAR				( 1 << 4 )		// 1 = clear fifo
#define HAL_BSC_CONTROL_STARTTRANSFER		( 1 << 7 )		// 1 = start new transfer
#define HAL_BSC_CONTROL_INT_DONE			( 1 << 8 )		// 1 = generate interrupt while DONE = 1
#define HAL_BSC_CONTROL_INT_TX				( 1 << 9 )		// 1 = generate interrupt while TXW = 1
#define HAL_BSC_CONTROL_INT_RX				( 1 << 10 )		// 1 = generate interrupt while RXR = 1
#define HAL_BSC_CONTROL_I2C_ENABLE			( 1 << 15 )		// 1 = BSC controller enabled
// status register
#define HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT	( 1 << 9 )		// 1 = slave holds SCL low
#define HAL_BSC_STATUS_ERROR				( 1 << 8 )		// 1 = slave has not ack
#define HAL_BSC_STATUS_RXFIFO_FULL			( 1 << 7 )		// 1 = FIFO is full
#define HAL_BSC_STATUS_TXFIFO_EMPTY			( 1 << 6 )		// 1 = FIFO is empty
#define HAL_BSC_STATUS_RXFIFO_NOTEMPTY		( 1 << 5 )		// 1 = FIFO contains at least 1 byte
#define HAL_BSC_STATUS_TXFIFO_HASSPACE		( 1 << 4 )		// 1 = FIFO has space for at least 1 byte
#define HAL_BSC_STATUS_RXFIFO_FULL_AR		( 1 << 3 )		// 1 = FIFO will be full after completing reading
#define HAL_BSC_STATUS_TXFIFO_EMPTY_AW		( 1 << 2 )		// 1 = FIFO will be empty after completing writing
#define HAL_BSC_STATUS_TRANSFER_DONE		( 1 << 1 )		// 1 = transfer complete
#define HAL_BSC_STATUS_TRANSFER_ACTIVE		( 1 << 0 )		// 1 = transfer active



static volatile hal_bsc_regs_t * const hal_bsc0_regs = ( hal_bsc_regs_t * ) ( HAL_BSC0_BASE );
static volatile hal_bsc_regs_t * const hal_bsc1_regs = ( hal_bsc_regs_t * ) ( HAL_BSC1_BASE );
static volatile hal_bsc_regs_t * const hal_bsc2_regs = ( hal_bsc_regs_t * ) ( HAL_BSC2_BASE );

// prototypes
static hal_error_status_t hal_bsc_Init( volatile hal_bsc_regs_t * BscController, hal_base_t I2CFrequency );
static hal_error_status_t hal_bsc_WriteTransaction(
	const uint8_t SlaveAddress,
	uint8_t * Buffer,
	uint8_t BufferSize,
	volatile hal_bsc_regs_t * BscController );
static hal_error_status_t hal_bsc_ReadTransaction(
	const uint8_t SlaveAddress,
	uint8_t * Buffer,
	uint8_t BufferSize,
	volatile hal_bsc_regs_t * BscController );



hal_error_status_t hal_bsc_Init_I2C0( hal_base_t I2CFrequency )
{
	// activate gpio alternative function for bsc, i2c
	if ( hal_gpio_SetPinFunction( HAL_GPIO_PIN_0, HAL_GPIO_FUNCSEL_ALT0 ) != HAL_ERROR_NO_ERROR )
	{
		// could not set pin function
		return HAL_ERROR_GENERAL_ERROR;
	}
	if ( hal_gpio_SetPinFunction( HAL_GPIO_PIN_1, HAL_GPIO_FUNCSEL_ALT0 ) != HAL_ERROR_NO_ERROR )
	{
		// could not set pin function
		return HAL_ERROR_GENERAL_ERROR;
	}

	return hal_bsc_Init( hal_bsc0_regs, I2CFrequency );
}
hal_error_status_t hal_bsc_WriteTransaction_I2C0(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize )
{
	return hal_bsc_WriteTransaction(
			SlaveAddress,
			Buffer,
			BufferSize,
			hal_bsc0_regs);
}
hal_error_status_t hal_bsc_ReadTransaction_I2C0(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize )
{
	return hal_bsc_ReadTransaction(
			SlaveAddress,
			Buffer,
			BufferSize,
			hal_bsc0_regs);
}
hal_error_status_t hal_bsc_Init_I2C1( hal_base_t I2CFrequency )
{
	// activate gpio alternative function for bsc, i2c
	if ( hal_gpio_SetPinFunction( HAL_GPIO_PIN_2, HAL_GPIO_FUNCSEL_ALT0 ) != HAL_ERROR_NO_ERROR )
	{
		// could not set pin function
		return HAL_ERROR_GENERAL_ERROR;
	}
	if ( hal_gpio_SetPinFunction( HAL_GPIO_PIN_3, HAL_GPIO_FUNCSEL_ALT0 ) != HAL_ERROR_NO_ERROR )
	{
		// could not set pin function
		return HAL_ERROR_GENERAL_ERROR;
	}

	return hal_bsc_Init( hal_bsc1_regs, I2CFrequency );
}
hal_error_status_t hal_bsc_WriteTransaction_I2C1(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize )
{
	return hal_bsc_WriteTransaction(
			SlaveAddress,
			Buffer,
			BufferSize,
			hal_bsc1_regs);
}
hal_error_status_t hal_bsc_ReadTransaction_I2C1(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize )
{
	return hal_bsc_ReadTransaction(
			SlaveAddress,
			Buffer,
			BufferSize,
			hal_bsc1_regs);
}


static hal_error_status_t hal_bsc_Init( volatile hal_bsc_regs_t * BscController, hal_base_t I2CFrequency )
{
	// init given bsc controller
	// the corresponding gpio pins have to be set to bsc alternative function separately

	hal_base_t clockDivider;

	// clear control register / disable protocol machine, needed bits will be set in following lines
	BscController->Control = 0;

	// clear timeout error flag
	BscController->Status |= HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT;
	// clear acknowledge error flag
	BscController->Status |= HAL_BSC_STATUS_ERROR;
	// clear done flag
	BscController->Status |= HAL_BSC_STATUS_TRANSFER_DONE;

	// clear no. of bytes register
	BscController->DataLength = 0;
	// clear slave address register
	BscController->SlaveAddress = 0;


	// debug:
	//clockDivider = BscController->ClockDivider;

	// set frequency
	// see https://elinux.org/BCM2835_datasheet_errata
	// core clock is 250MHz
	// I2CFrequency = CoreClock / ClockDivider
	// ClockDivider = CoreClock / I2CFrequency
	clockDivider = HAL_RPI_CORE_CLOCK_FREQUENCY / I2CFrequency;
	//clockDivider = 0x5DC;		// default value3
	BscController->ClockDivider = clockDivider;
	// delay values ( Number of core clock cycles to wait after the rising/falling edge of SCL before reading the next bit of data )
	BscController->DataDelay = 0x3030;			// default value
	// timeout on SCL high ( Number of SCL clock cycles to wait after the rising edge of SCL before deciding that the slave is not responding )
	BscController->ClkStretchTimeout = 0x40;	// default value

	// enable i2c protocol machine
	BscController->Control |= HAL_BSC_CONTROL_I2C_ENABLE;

	return HAL_ERROR_NO_ERROR;
}
static hal_error_status_t hal_bsc_WriteTransaction(
	const uint8_t SlaveAddress,
	uint8_t * Buffer,
	uint8_t BufferSize,
	volatile hal_bsc_regs_t * BscController )
{
	int i;

	if ( BufferSize > 16 )
	{
		// to much data
		return HAL_ERROR_GENERAL_ERROR;
	}

	if ( ( BscController->Status & HAL_BSC_STATUS_TRANSFER_ACTIVE ) != 0 )
	{
		// transaction already running
		return HAL_ERROR_GENERAL_ERROR;
	}

	// clear BSC & set address
	BscController->Status |= HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT;	// clear timeout error
	BscController->Status |= HAL_BSC_STATUS_ERROR;					// clear acknowledge error
	BscController->Status |= HAL_BSC_STATUS_TRANSFER_DONE;			// clear done flag
	BscController->Control |= HAL_BSC_CONTROL_CLEAR;				// clear fifo
	BscController->Control &= ~HAL_BSC_CONTROL_READ;				// write
	BscController->SlaveAddress = SlaveAddress;						// set slave address

	// fill fifo  max 16 bytes
	BscController->DataLength = BufferSize;
	for ( i = 0; i < BufferSize; i++ )
	{
		BscController->DataFifo = *Buffer++;
	}

	// start transaction
	BscController->Control |= HAL_BSC_CONTROL_STARTTRANSFER;
	do
	{
		if ( ( BscController->Status & HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT ) != 0 )
		{
			// timeout
			return HAL_ERROR_GENERAL_ERROR;
		}
		if ((BscController->Status & HAL_BSC_STATUS_ERROR) != 0)
		{
			// no slave acknowledge
			return HAL_ERROR_GENERAL_ERROR;
		}
	} while ( ( BscController->Status & HAL_BSC_STATUS_TRANSFER_DONE ) == 0 );	// while transfer active

	// clear done flag in status register
	BscController->Status |= HAL_BSC_STATUS_TRANSFER_DONE;

	return HAL_ERROR_NO_ERROR;
}
static hal_error_status_t hal_bsc_ReadTransaction(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize,
		volatile hal_bsc_regs_t * BscController )
{
	int i;

	// fifo size: 16 bytes
	if ( BufferSize > 16 )
	{
		// bigger than fifo
		return HAL_ERROR_GENERAL_ERROR;
	}

	// check if a transaction is running
	if ( ( BscController->Status & HAL_BSC_STATUS_TRANSFER_ACTIVE ) != 0 )
	{
		// transaction already running
		return HAL_ERROR_GENERAL_ERROR;
	}

	// clear BSC & set address
	BscController->Status |= HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT;	// clear timeout error
	BscController->Status |= HAL_BSC_STATUS_ERROR;					// clear acknowledge error
	BscController->Status |= HAL_BSC_STATUS_TRANSFER_DONE;			// clear done flag
	BscController->Control |= HAL_BSC_CONTROL_CLEAR;				// clear fifo
	BscController->Control |= HAL_BSC_CONTROL_READ;					// read
	BscController->SlaveAddress = SlaveAddress;						// set slave address

	// set number of receive bytes   max 16
	BscController->DataLength = BufferSize;

	// start transaction
	BscController->Control |= HAL_BSC_CONTROL_STARTTRANSFER;
	do {
		// check if slave timed out
		if ( ( BscController->Status & HAL_BSC_STATUS_CLK_STRETCH_TIMEOUT ) != 0 )
		{
			// timeout
			return HAL_ERROR_GENERAL_ERROR;
		}
		// check if slave acknowledged
		if ( ( BscController->Status & HAL_BSC_STATUS_ERROR ) != 0 )
		{
			// no slave acknowledge
			return HAL_ERROR_GENERAL_ERROR;
		}
	} while ( ( BscController->Status & HAL_BSC_STATUS_TRANSFER_DONE ) == 0 );	// while transfer active

	// clear done flag
	BscController->Status |= HAL_BSC_STATUS_TRANSFER_DONE;

	// read fifo
	for ( i = 0; i < BufferSize; i++ )
	{
		if ( ( BscController->Status & HAL_BSC_STATUS_RXFIFO_NOTEMPTY ) == 0 )
		{
			// fifo is empty
			return HAL_ERROR_GENERAL_ERROR;
		}
		*Buffer++ = BscController->DataFifo;
	}

	return 0;
}

