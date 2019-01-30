/*
 * rpi_hwBasic.h
 *
 *  Created on: 27.01.2019
 *      Author: Fabian
 */

#ifndef RPI_HWBASIC_H_
#define RPI_HWBASIC_H_


hal_base_t hal_hwBasic_GetSP(void);
hal_base_t hal_hwBasic_GetCPSR(void);

hal_base_t hal_hwBasic_Get32(hal_base_t address);
void hal_hwBasic_Put32(hal_base_t address, hal_base_t value);


#endif /* RPI_HWBASIC_H_ */