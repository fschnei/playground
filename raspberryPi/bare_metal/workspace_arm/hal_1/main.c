#include "rpi2.h"

int main()
{
	// Pegel von GPIO5 auf Null setzen
	*GPCLR0 = (1<<5);
	// GPIO 5 als Ausgang schalten
	*GPFSEL0 &= ~(GPIOFSEL_CLR << FSEL5);
	*GPFSEL0 |=	GPIOFSEL_OUT << FSEL5;

	volatile int i;

	while (1)
	{
		// GPIO umschalten
		if ( *GPLEV0 & (1<<5) )
			*GPCLR0 = (1<<5);
		else
			*GPSET0 = (1<<5);

		// Warteschleife
		i=0x100000;
		while (i>0)
			i--;
	}
}

