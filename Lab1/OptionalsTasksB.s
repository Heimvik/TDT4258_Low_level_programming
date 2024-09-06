//Task B: Word reversal
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

findLength:
    //Assumes
    // - Adress first byte in r0
    // - Returns length in r0
    push {lr}
    push {r4-r12}
    mov r3, #0
    mov r2, #0
    
    loopForLength:
        ldrb r1, [r0,r2]
        cmp r1, r3
        beq return
        add r2, r2, #1
        b loopForLength
    return:
        mov r0, r2
        pop {r4-r12}
        pop {lr}
        bx lr

init:
    ldr r0, =INPUT
    mov r4, r0
    bl findLength
    add r5, r4, r0
    b loopReverse


    loopReverse:
        sub r5, r5, #1
        mov r1, #1
        mov r0, r5
        bl writeToJtag
        cmp r5,r4
        beq exit
		b loopReverse
exit:
    b .



.section .data
JTAG_DATA_ADR:
    .word 0xFF201000
JTAG_CONTROL_ADR:
	.word 0xFF201004
DATA_ADR:
    .word 0xF000
INPUT:
    .asciz "Hello World!"