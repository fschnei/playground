
.section ".text.startup"

.global _start
.global _get_stack_pointer
.global _exception_table

// From the ARM ARM (Architecture Reference Manual). Make sure you get the
// ARMv5 documentation which includes the ARMv6 documentation which is the
// correct processor type for the Broadcom BCM2835. The ARMv6-M manuals
// available on the ARM website are for Cortex-M parts only and are very
// different.
//
// See ARM section A2.2 (Processor Modes)

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.equ	SCTLR_ENABLE_DATA_CACHE,        0x4
.equ	SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ	SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000

_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   undefined_instruction_vector
_software_interrupt_vector_h:       .word   software_interrupt_vector
_prefetch_abort_vector_h:           .word   prefetch_abort_vector
_data_abort_vector_h:               .word   data_abort_vector
_unused_handler_h:                  .word   unused_vector
_interrupt_vector_h:                .word   interrupt_vector
_fast_interrupt_vector_h:           .word   fast_interrupt_vector

_reset_:
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)

	// copy interrupt vector table from 0x8000 to 0x000
    mov     r0, #0x8000
    mov     r1, #0x0000

    // copy process
    // copy branch instructions
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    // copy IVT (interrupt vector table)
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

    // Initialise Stack Pointers ---------------------------------------------

    // We're going to use interrupt mode, so setup the interrupt mode
    // stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mrs	r0, cpsr
    mov sp, #0x7000

    // Switch back to supervisor mode (our application mode) and
    // set the stack pointer. Remember that the stack works its way
    // down memory, our heap will work it's way up from after the
    // application.
    mov r0, #( CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    //mov r0, #( CPSR_MODE_SVR )
    msr cpsr_c, r0
    mrs	r0, cpsr
    mov sp, #0x8000


    bl      _cstartup

_inf_loop:
    b       _inf_loop



_get_stack_pointer:
    // Return the stack pointer value
    str     sp, [sp]
    ldr     r0, [sp]

    // Return from the function
    mov     pc, lr

.globl PUT32
PUT32:
    str 	r1,[r0]
    mov     pc, lr

.globl GET32
GET32:
    ldr 	r0,[r0]
	mov     pc, lr


.global _enable_interrupts
_enable_interrupts:
// for processor modi see ARM ARM v7-A page 1139
// for registers per processor modi see ARM ARM v7-A page 1144
// for cpsr see ARM ARM v7-A page 1148
    mrs     r0, cpsr
    bic     r0, r0, #0x80
    msr     cpsr_c, r0

    mov     pc, lr


.globl hal_GetCPSR
hal_GetCPSR:
	mrs     r0, cpsr
	mov     pc, lr



undefined_instruction_vector:
	b undefined_instruction_vector

software_interrupt_vector:
	b software_interrupt_vector

prefetch_abort_vector:
	b prefetch_abort_vector

data_abort_vector:
	b data_abort_vector

unused_vector:
	b unused_vector

//interrupt_vector:
//	b interrupt_vector

fast_interrupt_vector:
	b fast_interrupt_vector

hang:
	b hang
