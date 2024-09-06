//TASK A:

.section .text
.global _start
_start:
    b init

.global writeToJtag
writeToJtag:
    //Assumes:
    // - The adress of the first byte lies in r0
    // - The length of the word lies in r1
    // - The return adress lies in lr
    // - No parameters to pop off the stack

    push {lr}
    push {r4-r12}
    mov r6,#0
    b checkSpaceInJtag

    checkSpaceInJtag:    
        ldr r3, =JTAG_CONTROL_ADR
        ldr r3, [r3]
        ldrb r4, [r3,#2]
        cmp r4, #0x08
        bhi writeByteToJtag
        b checkSpaceInJtag
    writeByteToJtag:
        ldr r3, =JTAG_DATA_ADR
        ldr r3, [r3]
        ldrb r5, [r0]
        strb r5, [r3]
        add r6,r6,#1
        cmp r6,r1
        beq returnFromJtag
        b writeByteToJtag
    returnFromJtag:
        pop {r4-r12}
        pop {lr}
        bx lr

init:
    ldr r0, =DATA_ADR
    ldr r0, [r0]
    mov r4, #0 //Keeps the value of our iterator
    mov r5, #100
    b loop

loop:
    strb r4, [r0]
    mov r1, #1
    bl writeToJtag

    add r4, r4, #1
    cmp r4, r5
    beq _exit
    ldr r6, = DATA_ADR
    ldr r6, [r6]
    add r0, r4, r6
    b loop

_exit:
    b .

.section .data
JTAG_DATA_ADR:
    .word 0xFF201000
JTAG_CONTROL_ADR:
	.word 0xFF201004
DATA_ADR:
    .word 0xF000
