
extern int __bss_start__;
extern int __bss_end__;

extern void application_main();

void _cstartup( unsigned int r0, unsigned int r1, unsigned int r2 )
{
    int* bss = &__bss_start__;
    int* bss_end = &__bss_end__;

    // clear bss section
    while( bss < bss_end )
        *bss++ = 0;

    // call application
    application_main();

    // catch if application_main returns
    while(1)
    {
        // nop
    }
}
