/*
 * asm_prototypes.h
 *
 *  Created on: 12.01.2019
 *      Author: Fabian
 */

#ifndef ASM_PROTOTYPES_H_
#define ASM_PROTOTYPES_H_


extern void PUT32 ( volatile unsigned long, volatile unsigned long );
extern volatile unsigned long GET32 ( volatile unsigned long );
extern void _enable_interrupts(void);

extern unsigned long hal_GetCPSR (void);


#endif /* ASM_PROTOTYPES_H_ */
