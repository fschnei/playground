/*
 * rpi_bsc.h
 *
 *  Created on: 04.02.2019
 *      Author: Fabian
 */

#ifndef RPI_BSC_H_
#define RPI_BSC_H_

#include "../rpi_base.h"


typedef struct{
	hal_reg_rw_t 	Control;
	hal_reg_rw_t 	Status;
    hal_reg_rw_t 	DataLength;
    hal_reg_rw_t 	SlaveAddress;
    hal_reg_rw_t 	DataFifo;
    hal_reg_rw_t 	ClockDivider;
    hal_reg_rw_t 	DataDelay;			// Number of core clock cycles to wait after the rising/falling edge of SCL before reading the next bit of data
    hal_reg_rw_t 	ClkStretchTimeout;	// Number of SCL clock cycles to wait after the rising edge of SCL before deciding that the slave is not responding
} hal_bsc_regs_t;



hal_error_status_t hal_bsc_Init_I2C0( void );

hal_error_status_t hal_bsc_WriteTransaction_I2C0(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize );



#endif // RPI_BSC_H_
