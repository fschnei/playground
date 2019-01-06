.globl main

#include "rpi2.h"



main:

// Pegel von GPIO5 auf Null setzen
ldr r0, =GPCLR0
mov r1, #1
lsl r1, #5
str r1, [r0]

// GPIO5 als Ausgang schalten
ldr r0, =GPFSEL0
ldr r2, [r0]
bic r2, r2, #(GPIOFSEL_CLR<<FSEL5)
orr r2, r2, #(GPIOFSEL_OUT<<FSEL5)
str r2, [r0]

loop:

// LED umschalten
// Pegel von GPIO5 abfragen
ldr r0, =GPLEV0
ldr r2, [r0]

// Testen, ob Pegel auf 1
mov r1, #1
lsl r1, #5
tst r2,r1

// Nein: Adresse fuer Anschalten laden
ldreq r0, =GPSET0
// Ja: Adresse fuer Abschalten laden
ldrne r0, =GPCLR0
// Ausfuehrung
str r1, [r0]

// Warteschleife
mov r0, #0x100000
wait:
	subs r0, #1
	bne wait

// wieder von vorne
b loop


