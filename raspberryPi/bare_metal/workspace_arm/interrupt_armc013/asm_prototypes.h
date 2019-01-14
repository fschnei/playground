
#ifndef ASM_PROTOTYPES_H_
#define ASM_PROTOTYPES_H_


extern void PUT32 ( volatile unsigned long, volatile unsigned long );
extern volatile unsigned long GET32 ( volatile unsigned long );
extern void _enable_interrupts(void);


#endif /* ASM_PROTOTYPES_H_ */
