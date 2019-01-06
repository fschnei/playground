/******************************************************************************
*   main.s
*
*   A sample assembly code implementation of the ok02 operating system, that
*   simply turns the OK LED on and off repeatedly.
*   Changes since OK01 are marked with NEW.
******************************************************************************/
.set GPFSEL0, 0x00
.set FSEL0, 0
.set FSEL1, 3
.set FSEL2, 6
.set FSEL3, 9
.set FSEL4, 12
.set FSEL5, 15
.set FSEL6, 18
.set FSEL7, 21
.set FSEL8, 24
.set FSEL9, 27

.set GPFSEL1, 0x04
.set FSEL10, 0
.set FSEL11, 3
.set FSEL12, 6
.set FSEL13, 9
.set FSEL14, 12
.set FSEL15, 15
.set FSEL16, 18
.set FSEL17, 21
.set FSEL18, 24
.set FSEL19, 27

.set GPFSEL2, 0x08
.set FSEL20, 0
.set FSEL21, 3
.set FSEL22, 6
.set FSEL23, 9
.set FSEL24, 12
.set FSEL25, 15
.set FSEL26, 18
.set FSEL27, 21
.set FSEL28, 24
.set FSEL29, 27

.set GPFSEL3, 0x0C
.set FSEL30, 0
.set FSEL31, 3
.set FSEL32, 6
.set FSEL33, 9
.set FSEL34, 12
.set FSEL35, 15
.set FSEL36, 18
.set FSEL37, 21
.set FSEL38, 24
.set FSEL39, 27

.set GPFSEL4, 0x10
.set FSEL40, 0
.set FSEL41, 3
.set FSEL42, 6
.set FSEL43, 9
.set FSEL44, 12
.set FSEL45, 15
.set FSEL46, 18
.set FSEL47, 21
.set FSEL48, 24
.set FSEL49, 27

.set GPFSEL5, 0x14
.set FSEL50, 0
.set FSEL51, 3
.set FSEL52, 6
.set FSEL53, 9

.set GPIOIN, 0x0
.set GPIOOUT, 0x1
.set GPIOALT0, 0x4
.set GPIOALT1, 0x5
.set GPIOALT2, 0x6
.set GPIOALT3, 0x7
.set GPIOALT4, 0x3
.set GPIOALT5, 0x2
.set GPIOCLRMSK, 0x7

.set GPPUD, 0x94
.set GPUDCLK0, 0x98

/*
* .section is a directive to our assembler telling it to place this code first.
* .globl is a directive to our assembler, that tells it to export this symbol
* to the elf file. Convention dictates that the symbol _start is used for the
* entry point, so this all has the net effect of setting the entry point here.
* Ultimately, this is useless as the elf itself is not used in the final
* result, and so the entry point really doesn't matter, but it aids clarity,
* allows simulators to run the elf, and also stops us getting a linker warning
* about having no entry point.
*/
.section .init
.globl _start
_start:


/*
* This command loads the physical address of the GPIO region into r0.
*/
//ldr r0,=0x20200000 // RPI 1
    ldr r0,=0x3F200000 // RPI 2
    mov r2, #1

    mov r1, #0
    str r1, [r0, #GPPUD]
    bl dowait                       // wait 150 cycles as per datasheet

    mov r1, #0
    orr r1, r1, r2, lsl #22
    orr r1, r1, r2, lsl #23
    orr r1, r1, r2, lsl #24
    orr r1, r1, r2, lsl #25
    orr r1, r1, r2, lsl #26
    orr r1, r1, r2, lsl #27
    str r1, [r0, #GPUDCLK0]
    bl dowait

    mov r1, #0
    str r1, [r0, #GPUDCLK0]
    bl dowait

    mov r2, #GPIOCLRMSK
    ldr r2, [r0, #GPFSEL2]
    bic r1, r1, r2, lsl #FSEL22
    bic r1, r1, r2, lsl #FSEL23
    bic r1, r1, r2, lsl #FSEL24
    bic r1, r1, r2, lsl #FSEL25
    bic r1, r1, r2, lsl #FSEL26
    bic r1, r1, r2, lsl #FSEL27
    mov r2, #GPIOALT4
    orr r1, r1, r2, lsl #FSEL22
    orr r1, r1, r2, lsl #FSEL23
    orr r1, r1, r2, lsl #FSEL24
    orr r1, r1, r2, lsl #FSEL25
    orr r1, r1, r2, lsl #FSEL26
    orr r1, r1, r2, lsl #FSEL27
    str r1, [r0, #GPFSEL2]

endless_loop:
	b endless_loop		// freez and wait for jtag input

dowait:
   mov r2, #300
dowaitloop:
   subs r2, #1
   bne dowaitloop
   bx lr

