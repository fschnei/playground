
#ifndef RPI_BASE_H
#define RPI_BASE_H

#include "../int_types.h"

// see: https://www.raspberrypi.org/documentation/hardware/raspberrypi/peripheral_addresses.md
#ifdef RPI2
    #define HAL_RPI_PERIPHERAL_BASE     0x3F000000UL
#else
    #define HAL_RPI_PERIPHERAL_BASE     0x20000000UL
#endif

typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;


// volatile types

typedef volatile uint32_t rpi_volatile_reg_t;


// set clear helper macros
#define REGISTER_SET 0xFFFFFFFFUL
#define REGISTER_CLR 0x00000000UL

#endif
