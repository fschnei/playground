
#ifndef RPI_AUX_H
#define RPI_AUX_H

#include "../rpi_base.h"

/* Although these values were originally from the BCM2835 Arm peripherals PDF
   it's clear that was rushed and has some glaring errors - so these values
   may appear to be different. These values have been changed due to data on
   the elinux BCM2835 datasheet errata:
   http://elinux.org/BCM2835_datasheet_errata */


// !!! errata:
// http://elinux.org/BCM2835_datasheet_errata

#define HAL_AUXILIARIES_BASE    	( HAL_RPI_PERIPHERAL_BASE + 0x215000 )

#define AUX_ENA_MINIUART            ( 1 << 0 )
#define AUX_ENA_SPI1                ( 1 << 1 )
#define AUX_ENA_SPI2                ( 1 << 2 )

#define AUX_IRQ_SPI2                ( 1 << 2 )
#define AUX_IRQ_SPI1                ( 1 << 1 )
#define AUX_IRQ_MU                  ( 1 << 0 )

#define AUX_MULCR_8BIT_MODE         ( 3 << 0 )  /* See errata for this value */
#define AUX_MULCR_BREAK             ( 1 << 6 )
#define AUX_MULCR_DLAB_ACCESS       ( 1 << 7 )

#define AUX_MUMCR_RTS               ( 1 << 1 )

#define AUX_MULSR_DATA_READY        ( 1 << 0 )
#define AUX_MULSR_RX_OVERRUN        ( 1 << 1 )
#define AUX_MULSR_TX_EMPTY          ( 1 << 5 )
#define AUX_MULSR_TX_IDLE           ( 1 << 6 )

#define AUX_MUMSR_CTS               ( 1 << 5 )

#define AUX_MUCNTL_RX_ENABLE        ( 1 << 0 )
#define AUX_MUCNTL_TX_ENABLE        ( 1 << 1 )
#define AUX_MUCNTL_RTS_FLOW         ( 1 << 2 )
#define AUX_MUCNTL_CTS_FLOW         ( 1 << 3 )
#define AUX_MUCNTL_RTS_FIFO         ( 3 << 4 )
#define AUX_MUCNTL_RTS_ASSERT       ( 1 << 6 )
#define AUX_MUCNTL_CTS_ASSERT       ( 1 << 7 )

#define AUX_MUSTAT_SYMBOL_AV        ( 1 << 0 )
#define AUX_MUSTAT_SPACE_AV         ( 1 << 1 )
#define AUX_MUSTAT_RX_IDLE          ( 1 << 2 )
#define AUX_MUSTAT_TX_IDLE          ( 1 << 3 )
#define AUX_MUSTAT_RX_OVERRUN       ( 1 << 4 )
#define AUX_MUSTAT_TX_FIFO_FULL     ( 1 << 5 )
#define AUX_MUSTAT_RTS              ( 1 << 6 )
#define AUX_MUSTAT_CTS              ( 1 << 7 )
#define AUX_MUSTAT_TX_EMPTY         ( 1 << 8 )
#define AUX_MUSTAT_TX_DONE          ( 1 << 9 )
#define AUX_MUSTAT_RX_FIFO_LEVEL    ( 7 << 16 )
#define AUX_MUSTAT_TX_FIFO_LEVEL    ( 7 << 24 )


#define FSEL0(x)        ( x )
#define FSEL1(x)        ( x << 3 )
#define FSEL2(x)        ( x << 6 )
#define FSEL3(x)        ( x << 9 )
#define FSEL4(x)        ( x << 12 )
#define FSEL5(x)        ( x << 15 )
#define FSEL6(x)        ( x << 18 )
#define FSEL7(x)        ( x << 21 )
#define FSEL8(x)        ( x << 24 )
#define FSEL9(x)        ( x << 27 )

#define FSEL10(x)       ( x )
#define FSEL11(x)       ( x << 3 )
#define FSEL12(x)       ( x << 6 )
#define FSEL13(x)       ( x << 9 )
#define FSEL14(x)       ( x << 12 )
#define FSEL15(x)       ( x << 15 )
#define FSEL16(x)       ( x << 18 )
#define FSEL17(x)       ( x << 21 )
#define FSEL18(x)       ( x << 24 )
#define FSEL19(x)       ( x << 27 )

#define FSEL20(x)       ( x )
#define FSEL21(x)       ( x << 3 )
#define FSEL22(x)       ( x << 6 )
#define FSEL23(x)       ( x << 9 )
#define FSEL24(x)       ( x << 12 )
#define FSEL25(x)       ( x << 15 )
#define FSEL26(x)       ( x << 18 )
#define FSEL27(x)       ( x << 21 )
#define FSEL28(x)       ( x << 24 )
#define FSEL29(x)       ( x << 27 )

#define FSEL30(x)       ( x )
#define FSEL31(x)       ( x << 3 )
#define FSEL32(x)       ( x << 6 )
#define FSEL33(x)       ( x << 9 )
#define FSEL34(x)       ( x << 12 )
#define FSEL35(x)       ( x << 15 )
#define FSEL36(x)       ( x << 18 )
#define FSEL37(x)       ( x << 21 )
#define FSEL38(x)       ( x << 24 )
#define FSEL39(x)       ( x << 27 )

#define FSEL40(x)       ( x )
#define FSEL41(x)       ( x << 3 )
#define FSEL42(x)       ( x << 6 )
#define FSEL43(x)       ( x << 9 )
#define FSEL44(x)       ( x << 12 )
#define FSEL45(x)       ( x << 15 )
#define FSEL46(x)       ( x << 18 )
#define FSEL47(x)       ( x << 21 )
#define FSEL48(x)       ( x << 24 )
#define FSEL49(x)       ( x << 27 )

#define FSEL50(x)       ( x )
#define FSEL51(x)       ( x << 3 )
#define FSEL52(x)       ( x << 6 )
#define FSEL53(x)       ( x << 9 )


// TODO: check this stuff (register ordering, count of reserved bytes)
typedef struct {
	hal_reg_r_t Irq;				// 0xXX215000
	hal_reg_rw_t Enable;			// 0xXX215004

    // create offset
	hal_reg_rw_t reserved1[((0x40 - 0x04) / 4) - 1];	// 0xXX215008 - 0xXX21503C

	// mini uart
    hal_reg_rw_t MiniUart_Io;		// 0xXX215040
    hal_reg_rw_t MiniUart_IER;		// 0xXX215044	// http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t MiniUart_IIR;		// 0xXX215048	// http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t MiniUart_LCR;		// 0xXX21504C
    hal_reg_rw_t MiniUart_MCR;		// 0xXX215050
    hal_reg_r_t MiniUart_LSR;		// 0xXX215054
    hal_reg_r_t MiniUart_MSR;		// 0xXX215058
    hal_reg_rw_t MiniUart_SCRATCH;	// 0xXX21505C
    hal_reg_rw_t MiniUart_CNTL;		// 0xXX215060
    hal_reg_r_t MiniUart_STAT;		// 0xXX215064
    hal_reg_rw_t MiniUart_BAUD;		// 0xXX215068

    // create offset
    hal_reg_rw_t reserved2[(0x80 - 0x68) / 4];			// 0xXX21506C - 0xXX21507C

    // SPI 0
    hal_reg_rw_t SPI1_CNTL0;		// 0xXX215080
    hal_reg_rw_t SPI1_CNTL1;		// 0xXX215084
    hal_reg_rw_t SPI1_STAT;			// 0xXX215088
    hal_reg_rw_t reserved2_;		// 0xXX21508C
    hal_reg_r_t SPI1_IO;			// 0xXX215090 or 0xXX21508C ? // http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t SPI1_PEEK;			// 0xXX215094 or 0xXX215090 ? // http://elinux.org/BCM2835_datasheet_errata

    // create offset
    hal_reg_rw_t reserved3[(0xC0 - 0x94) / 4];			// 0xXX2150A4 - 0xXX2150BC

    // SPI 1
    hal_reg_rw_t SPI2_CNTL0;		// 0xXX2150C0
    hal_reg_rw_t SPI2_CNTL1;		// 0xXX2150C4
    hal_reg_rw_t SPI2_STAT;			// 0xXX2150C8
    hal_reg_rw_t reserved3_;		// 0xXX2150CC
    hal_reg_r_t SPI2_IO;			// 0xXX2150D0 or 0xXX2150CC ? // http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t SPI2_PEEK;			// 0xXX2150D4 or 0xXX2150D0 ? // http://elinux.org/BCM2835_datasheet_errata
} hal_auxiliaries_regs_t;


hal_auxiliaries_regs_t * hal_auxiliaries_GetRegs( void );

void hal_auxiliaries_MiniUartInit( int baud, int bits );

void hal_auxiliaries_MiniUartWrite( char c );


#endif // RPI_AUX_H
