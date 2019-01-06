/*
 * hal.h
 *
 *  Created on: 23.01.2018
 *      Author: fasch019
 */

#ifndef HAL_H_
#define HAL_H_


// function switchs an defined LED (LED_index) to the new LED state (LED_state)
// LED_state:
//          0: LED off
//          1: LED on
// LED_index:
//          0, 1, ..., 7
// return value:
//          0: ok
//          0xFF: error (LED not available, LED port is no output)
unsigned char hal_switchLED(unsigned char LED_index, unsigned char LED_state);


// function returns the current state of the given switch with Switch_index
// Switch_index:
//          0, 1, 2, 3
// return value:
//          0: switch is off
//          1: switch is on
//          0xFF: error (switch not available, switch port is no input)
unsigned char hal_getSwitchState(unsigned char Switch_index);


// function returns the current state of the given button with Button_index
// Button_index:
//          0, 1, 2, 3
// return value:
//          0: button is off
//          1: button is on
//          0xFF: error (button not available, button port is no input)
unsigned char hal_getButtonState(unsigned char Button_index);





// function sets the direction of LED port, switch port and button port
void hal_init_system(void);



// this functions waits for circa! 1ms
// the time depends on target board (msp430 should be precise)
void hal_wait_1ms(void);


#endif /* HAL_H_ */
