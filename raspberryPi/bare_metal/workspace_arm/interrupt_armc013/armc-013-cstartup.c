/*
    Part of the Raspberry-Pi Bare Metal Tutorials
    https://www.valvers.com/rpi/bare-metal/
    Copyright (c) 2013-2018, Brian Sidebotham

    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.

*/

extern void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags );

void _cstartup( unsigned int r0, unsigned int r1, unsigned int r2 )
{
    /* We should never return from main ... */
    kernel_main( r0, r1, r2 );

    /* ... but if we do, safely trap here */
    while(1)
    {
        /* EMPTY! */
    }
}
