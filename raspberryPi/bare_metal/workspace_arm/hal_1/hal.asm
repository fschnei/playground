
.global hal_init_system
.global hal_wait_1ms
.global hal_switchLED
.global hal_getButtonState
.global hal_getSwitchState

#include "rpi2.h"




// function returns the current state of the given switch with Switch_index
// Switch_index:	(r0)
//          0, 1, 2, 3
// return value:	(r0)
//          0: switch is off
//          1: switch is on
//          0xFF: error (switch not available, switch port is no input)
//unsigned char hal_getSwitchState(unsigned char Switch_index);
hal_getSwitchState:
// TODO merge with getButtonState -> GetInputState	(LEDs -> SetOutputLevel)
	push {r1}
	push {r2}
	push {r3}
	// TODO: check range of LED Switch_index
	// check if its GPIO2,3 or GPIO16,17

//	mov r1, #0	//is it Switch_index 0?
	//tst r0, r1
	//beq hal_getSwitchState_execute1stPart

	mov r1, #1	//is it Switch_index 1?
	cmp r0, r1	// r0 - r1
	ble hal_getSwitchState_execute1stPart	// branch if r0 is lower than or equal 1

	// if not than ist the 2nd part of pins
	b hal_getSwitchState_execute2ndPart

hal_getSwitchState_execute1stPart:
	// get level from gpio
	ldr r3, =GPLEV0
	ldr r2, [r3]	// get shadow from input port
	// test, level on shadow
	mov r1, #1
	lsl r1, #2		// set offset from 0 (no Switch) to 2 (GPIO2, Switch1)
	lsl r1, r0		// rotate again to select Switch
	tst r2, r1		// test level (bit r1) on shadow from input port
	// no: return 0
	moveq r0, #0
	// yes: return 1
	movne r0, #1
	b hal_getSwitchState_finsihed

hal_getSwitchState_execute2ndPart:
	// get level from gpio
	ldr r3, =GPLEV0
	ldr r2, [r3]	// get shadow from input port
	// test, level on shadow
	mov r1, #1
	lsl r1, #16		// set offset from 0 (no Input) to 16 (GPIO16, Switch1)
	sub r0, r0, #2	// correct offeset from parameter
	lsl r1, r0		// rotate again to select Switch
	tst r2, r1		// test level (bit r1) on shadow from input port
	// no: return 0
	moveq r0, #0
	// yes: return 1
	movne r0, #1
	b hal_getSwitchState_finsihed

hal_getSwitchState_finsihed:
	pop {r3}
	pop {r2}
	pop {r1}
	// jump back to caller
	mov pc, lr



// function returns the current state of the given button with Button_index
// Button_index: (r0)
//          0, 1, 2, 3
// return value:	(r0)
//          0: button is off
//          1: button is on
//          0xFF: error (button not available, button port is no input)
//unsigned char hal_getButtonState(unsigned char Button_index);
hal_getButtonState:
	// seems to work

	push {r1}
	push {r2}
	push {r3}
	// TODO: check range of LED Button_index

	// get level from gpio
	ldr r3, =GPLEV0
	ldr r2, [r3]	// get shadow from input port

	// test, level on shadow
	mov r1, #1
	lsl r1, #18		// set offset from 0 (no button) to 18 (GPIO18, Button1)
	lsl r1, r0		// rotate again to select Button
	tst r2, r1		// test level (bit r1) on shadow from input port

	// no: return 0
	moveq r0, #0
	// yes: return 1
	movne r0, #1

hal_getButtonState_finsihed:
	pop {r3}
	pop {r2}
	pop {r1}
	// jump back to caller
	mov pc, lr






// function switchs an defined LED (LED_index) to the new LED state (LED_state)
// LED_state:	(r1)
//          0: LED off
//          1: LED on
// LED_index:	(r0)
//          0, 1, ..., 7
// return value:
//          0: ok
//          0xFF: error (LED not available, LED port is no output)
//unsigned char hal_switchLED(unsigned char LED_index, unsigned char LED_state);
hal_switchLED:
	push {r2}
	push {r3}
	// TODO: check range of LED index
	// TODO: check value

	// check wich led state caller want
	mov r2, #1	//is it "set LED off"?
	tst r1, r2
	beq hal_switchLED_off

	//mov r2, #1	//is it "set LED on"?
	//tst r2, r1
	//beq hal_switchLED_off

hal_switchLED_on:
	ldr r2, =GPSET0
	b hal_switchLED_action

hal_switchLED_off:
	ldr r2, =GPCLR0
	b hal_switchLED_action

hal_switchLED_action:
	mov r3, #1
	lsl r3, #5	// set offset from 0 (no LED) to 5 (GPIO5, LED1)
	lsl r3, r0	// rotate again to select LED
	str r3, [r2]
	b hal_switchLED_finsihed

hal_switchLED_finsihed:
	pop {r3}
	pop {r2}
	// jump back to caller
	mov pc, lr




hal_init_system:
// seems to work

/*SetGpioFunction(u32 gpioRegister, u32 function)
	mov r0,#16
	mov r1,#1
	bl SetGpioFunction

	mov r0,#16
	mov r1,#0
	bl SetGpio*/

	/*mov r0,	#5			// select GPIO5
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#6			// select GPIO6
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#7			// select GPIO7
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#8			// select GPIO8
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#9			// select GPIO9
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#10			// select GPIO10
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#11			// select GPIO11
	mov r1, #1			// set to output
	bl SetGpioFunction
	mov r0,	#12			// select GPIO12
	mov r1, #1			// set to output
	bl SetGpioFunction*/

	/*mov r0,	#5			// select GPIO5
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#6			// select GPIO6
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#7			// select GPIO7
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#8			// select GPIO8
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#9			// select GPIO9
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#10			// select GPIO10
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#11			// select GPIO11
	mov r1, #0			// set to input
	bl SetGpioFunction
	mov r0,	#12			// select GPIO12
	mov r1, #0			// set to input
	bl SetGpioFunction*/

	/*// Pegel von GPIO5 - GPIO12 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #0x1FE0
	str r1, [r0]*/

	// Pegel von GPIO5 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #5
	str r1, [r0]
	// Pegel von GPIO6 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #6
	str r1, [r0]
	// Pegel von GPIO7 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #7
	str r1, [r0]
	// Pegel von GPIO8 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #8
	str r1, [r0]
	// Pegel von GPIO9 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #9
	str r1, [r0]
	// Pegel von GPIO10 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #10
	str r1, [r0]
	// Pegel von GPIO11 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #11
	str r1, [r0]
	// Pegel von GPIO12 auf Null setzen
	ldr r0, =GPCLR0
	mov r1, #1
	lsl r1, #12
	str r1, [r0]


	/*// GPIO5 - GPIO #9 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL5 || GPIOFSEL_CLR<<FSEL6 || GPIOFSEL_CLR<<FSEL7 || GPIOFSEL_CLR<<FSEL8 || GPIOFSEL_CLR<<FSEL9)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL5 || GPIOFSEL_CLR<<FSEL6 || GPIOFSEL_CLR<<FSEL7 || GPIOFSEL_CLR<<FSEL8 || GPIOFSEL_CLR<<FSEL9)
	str r2, [r0]
	// GPIO10 - GPIO12 als Ausgang schalten
	ldr r0, =GPFSEL1
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL10 || GPIOFSEL_CLR<<FSEL11 || GPIOFSEL_CLR<<FSEL12)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL10 || GPIOFSEL_CLR<<FSEL11 || GPIOFSEL_CLR<<FSEL12)
	str r2, [r0]*/
	// GPIO5 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL5)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL5)
	str r2, [r0]
	// GPIO6 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL6)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL6)
	str r2, [r0]
	// GPIO7 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL7)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL7)
	str r2, [r0]
	// GPIO8 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL8)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL8)
	str r2, [r0]
	// GPIO9 als Ausgang schalten
	ldr r0, =GPFSEL0
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL9)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL9)
	str r2, [r0]
	// GPIO10 als Ausgang schalten
	ldr r0, =GPFSEL1
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL10)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL10)
	str r2, [r0]
	// GPIO11 als Ausgang schalten
	ldr r0, =GPFSEL1
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL11)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL11)
	str r2, [r0]
	// GPIO12 als Ausgang schalten
	ldr r0, =GPFSEL1
	ldr r2, [r0]
	bic r2, r2, #(GPIOFSEL_CLR<<FSEL12)
	orr r2, r2, #(GPIOFSEL_OUT<<FSEL12)
	str r2, [r0]

	// jump back to caller
	mov pc, lr



hal_wait_1ms:
	// Warteschleife
	mov r0, #0x1000
hal_wait_1ms_wait:
	subs r0, #1
	bne hal_wait_1ms_wait
	// jmp back to caller
	mov pc, lr

