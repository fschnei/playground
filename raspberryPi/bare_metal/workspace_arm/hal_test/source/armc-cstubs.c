
/* Prototype for the UART write function */
#include "rpi-aux.h"


/* A helper function written in assembler to aid us in allocating memory */
extern char * _get_stack_pointer(void);




void outbyte( char b )
{
    RPI_AuxMiniUartWrite( b );
}

/* Write to a file. libc subroutines will use this system routine for output to
   all files, including stdout—so if you need to generate any output, for
   example to a serial port for debugging, you should make your minimal write
   capable of doing this. The following minimal implementation is an
   incomplete example; it relies on a outbyte subroutine (not shown; typically,
   you must write this in assembler from examples provided by your hardware
   manufacturer) to actually perform the output. */
int _write( int file, char *ptr, int len )
{
    int todo;

    for( todo = 0; todo < len; todo++ )
      outbyte(*ptr++);

    return len;
}
