
.arch armv7ve
.section .text
.global _start

_start:
    b main



/*
Assumes:
Inputs:
r0: The number to display
r1: The position to display the number
*/
displaySingleNumber:
    push {lr}
    push {r4}

    ldr r2, =segmentConstants
    ldrb r2, [r2,r0]
    mov r3, #0xFF
    and r2,r2,r3
    cmp r1, #3
    ble setupLowerSeg7
    b setupUpperSeg7

    setupLowerSeg7:
        ldr r0, =seg7_adr_L
        b writeSeg7

    setupUpperSeg7:
        sub r1,r1,#4
        ldr r0, =seg7_adr_H
        b writeSeg7

    writeSeg7:
        ldr r4, [r0]
        mov r0, #8
        mul r1, r1, r0

        lsl r2, r2, r1 //r2 now keeps the current bits to set
        mov r3, #0xFF
        lsl r3, r3, r1 //r3 keeps the mask resetting
        mvn r3, r3

        ldr r0,[r4]
        and r0,r0,r3
        orr r0,r0,r2
        str r0,[r4]

        pop {r4}
        pop {pc}
    

/*
Assumes:
Inputs:
r0: The 32-bit number to display
Outputs:

The algorithm when converting from a bit-value to a decimal value, is:
1) repeated divtion by 10
2) store the remainder
3) check is remainder is 0, if not -> (1
*/
displayMultipleNumbers:
    push {lr}
    push {r4}
    mov r4, #0
    loopFindNextNumber:
        mov r1, #10
        udiv r2, r0, r1  
        mls  r3, r2, r1, r0    //R0 - (R2 * R1)
        push {r0-r3}
        mov r0,r3
        mov r1,r4
        bl displaySingleNumber
        pop {r0-r3}
        add r4, r4,#1  
        MOV  R0, R2           
        CMP  R0, #0            
        BNE  loopFindNextNumber
    pop {r4}
    pop {pc}

clearSeg7:
    push {lr}
    
    ldr r0, =seg7_adr_L
    ldr r1, =seg7_adr_H
    ldr r0,[r0]
    ldr r1,[r1]

    mov r2, #0x0
    str r2, [r0]
    str r2, [r1]
    
    pop {pc}


/*
Init TC to count upwards with a give frequency
Assumes:
r0: The frequency of the TC
*/
initTC:



main:
    mov r0,#341
    push {r0}
    bl clearSeg7
    pop {r0}
    bl displayMultipleNumbers

    b .

.section .data
seg7_adr_L: .word 0xFF200020
seg7_adr_H: .word 0xFF200030

tc_adr: .word 0xFFD00000
tc_load_offset: .word 0
tc_current_value_offset: .word 4
tc_control_offset: .word 8
tc_status_offset: .word 8

segmentConstants:
    .byte 0b00111111
    .byte 0b00000110 
    .byte 0b01011011  
    .byte 0b01001111
    .byte 0b01100110  
    .byte 0b01101101 
    .byte 0b01111101 
    .byte 0b00000111
    .byte 0b01111111 
    .byte 0b01101111 