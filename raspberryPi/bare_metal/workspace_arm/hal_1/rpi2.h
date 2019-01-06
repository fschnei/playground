/*
 * rpi2.h
 *
 *  Created on: 13.05.2015
 *      Author: Christian Puetter
 *      Author: Jan Richling
 */

#ifndef RPI2_H_
#define RPI2_H_

#ifdef __ASSEMBLER__
#define PTR
#else
#define PTR (volatile unsigned long*)
#endif

#define PeriBase 0x3F000000
#define GPIOBase (PeriBase+0x00200000)

#define GPIOFSEL_CLR 7

#define GPIOFSEL_IN 0
#define GPIOFSEL_OUT 1
#define GPIOFSEL_ALT0 4
#define GPIOFSEL_ALT1 5
#define GPIOFSEL_ALT2 6
#define GPIOFSEL_ALT3 7
#define GPIOFSEL_ALT4 3
#define GPIOFSEL_ALT5 2

#define GPFSEL0	PTR (GPIOBase+0x00)
#define FSEL0 0
#define FSEL1 3
#define FSEL2 6
#define FSEL3 9
#define FSEL4 12
#define FSEL5 15
#define FSEL6 18
#define FSEL7 21
#define FSEL8 24
#define FSEL9 27

#define GPFSEL1	PTR (GPIOBase+0x04)
#define FSEL10 0
#define FSEL11 3
#define FSEL12 6
#define FSEL13 9
#define FSEL14 12
#define FSEL15 15
#define FSEL16 18
#define FSEL17 21
#define FSEL18 24
#define FSEL19 27

#define GPFSEL2	PTR (GPIOBase+0x08)
#define FSEL20 0
#define FSEL21 3
#define FSEL22 6
#define FSEL23 9
#define FSEL24 12
#define FSEL25 15
#define FSEL26 18
#define FSEL27 21
#define FSEL28 24
#define FSEL29 27

#define GPFSEL3	PTR (GPIOBase+0x0C)
#define FSEL30 0
#define FSEL31 3
#define FSEL32 6
#define FSEL33 9
#define FSEL34 12
#define FSEL35 15
#define FSEL36 18
#define FSEL37 21
#define FSEL38 24
#define FSEL39 27

#define GPFSEL4	PTR (GPIOBase+0x10)
#define FSEL40 0
#define FSEL41 3
#define FSEL42 6
#define FSEL43 9
#define FSEL44 12
#define FSEL45 15
#define FSEL46 18
#define FSEL47 21
#define FSEL48 24
#define FSEL49 27

#define GPFSEL5	PTR (GPIOBase+0x14)
#define FSEL50 0
#define FSEL51 3
#define FSEL52 6
#define FSEL53 9

#define GPSET0 PTR (GPIOBase+0x1C)
#define GPSET1 PTR (GPIOBase+0x20)

#define GPCLR0 PTR (GPIOBase+0x28)
#define GPCLR1 PTR (GPIOBase+0x2C)

#define GPLEV0 PTR (GPIOBase+0x34)
#define GPLEV1 PTR (GPIOBase+0x38)


#define IRQBase (PeriBase + 0xB000)

#define IRQPend0 PTR (IRQBase + 0x200)
#define IRQPend1 PTR (IRQBase + 0x204)
#define IRQPend2 PTR (IRQBase + 0x208)
#define FIQCtrl PTR (IRQBase + 0x20c)
#define IRQEn1 PTR (IRQBase+ 0x210)
#define IRQEn2 PTR (IRQBase+ 0x214)
#define IRQEn0 PTR (IRQBase+ 0x218)
#define IRQDis1 PTR (IRQBase+ 0x21c)
#define IRQDis2 PTR (IRQBase+ 0x220)
#define IRQDis0 PTR (IRQBase+ 0x224)

#define ARM_TIMER_IRQ         0
#define ARM_MAILBOX_IRQ       1
#define ARM_DOORBELL_0_IRQ    2
#define ARM_DOORBELL_1_IRQ    3
#define GPU_0_HALTED_IRQ      4
#define GPU_1_HALTED_IRQ      5
#define ACCESS_ERROR_1_IRQ    6
#define ACCESS_ERROR_0_IRQ    7


#define ARMTimerBase (PeriBase + 0xB000)

#define ARMTimerLoad PTR (ARMTimerBase + 0x400)
#define ARMTimerValue PTR (ARMTimerBase + 0x404)
#define ARMTimerControl PTR (ARMTimerBase + 0x408)
#define ARMTimerIRQClear PTR (ARMTimerBase + 0x40c)
#define ARMTimerIRQRaw PTR (ARMTimerBase + 0x410)
#define ARMTimerIRQMask PTR (ARMTimerBase + 0x414)
#define ARMTimerReload PTR (ARMTimerBase + 0x418)
#define ARMTimerPredivider PTR (ARMTimerBase + 0x41c)
#define ARMTimerFreerunningcounter PTR (ARMTimerBase + 0x420)

#define ARMTimerControl_23BIT         ( 1 << 1 )
#define ARMTimerControl_PRESCALE_1    ( 0 << 2 )
#define ARMTimerControl_PRESCALE_16   ( 1 << 2 )
#define ARMTimerControl_PRESCALE_256  ( 2 << 2 )
#define ARMTimerControl_INT_ENABLE    ( 1 << 5 )
#define ARMTimerControl_INT_DISABLE   ( 0 << 5 )
#define ARMTimerControl_ENABLE        ( 1 << 7 )
#define ARMTimerControl_DISABLE       ( 0 << 7 )

#endif /* RPI2_H_ */
