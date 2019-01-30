
#ifndef RPI_BASE_H
#define RPI_BASE_H

#include "../int_types.h"


// see: https://www.raspberrypi.org/documentation/hardware/raspberrypi/peripheral_addresses.md
#if defined(RPI_2_VERSION_1_1) || defined(RPI_2_VERSION_1_2)
    #define HAL_RPI_PERIPHERAL_BASE     0x3F000000UL
	#define SYS_FREQ    				250000000
#else
    #define HAL_RPI_PERIPHERAL_BASE     0x20000000UL
	#define SYS_FREQ    				250000000
#endif



// word on this system (raspberry)
typedef uint32_t  hal_base_t;
typedef volatile hal_base_t hal_v_base_t;

// raspberry register types
typedef hal_v_base_t hal_reg_rw_t;
typedef hal_v_base_t hal_reg_w_t;
typedef const hal_v_base_t hal_reg_r_t;


#endif
