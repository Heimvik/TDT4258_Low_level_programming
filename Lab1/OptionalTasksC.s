//Load all values to the stack
//b to govenor

//govenor
//-receives number of elements in r0
//-keeps:
//  - li elements popped

//-outerloop
//-gives numbe of elements left on stack (num-num popped) in r0 to innerLoop
//-call to innerlooop
//-updates elements left
//-checks

//innerLoop
//-Popps d and number of elements on stack to adress
//-Two ponters checking the first one against the rest with cmp
//-Pushes backmost numleft-1 back on stack before retuning to govenor


.section .text
.global _start
_start:
    b main

main:
    //Start to push all values to stack
    ldr r0, =LI
    ldr r1, =D
    ldr r12,=LI_LENGTH
    ldrb r12,[r12]
    mov r5,#0

    b pushLI

    pushLI:
        ldrb r3, [r0,r5]
        push {r3}
        add r5,r5,#1
        cmp r5,r12
        bne pushLI
        b pushD
    pushD:
        ldrb r4, [r1]
        push {r4}
		b algo
	algo:
        //At this point, all values are on the stack, no values in registers needed (exept r12)
		pop {r0} //Holds D
        pop {r1} //Holds upper element in li
		sub r11,r12,#2
        mov r5, #0 //Holds the offset the last elment has from a full stack
        mov r3, #0 //Holds current element on the manipulated stack
        ldr r10, =DATA_INDEX
        ldr r9, =DATA_MATCHES
        b algoLoop1
        algoLoop1:
			pop {r2}
            add r3, r3, #1
            add r4,r1,r2
            cmp r4,r0
            beq algoMatchFound
            cmp r3,r11
            beq algoEndOfStack
            b algoLoop
            algoMatchFound:
                strb r3,[r10,r3] //Read as at positon r5 in the list, it matches with
        //Then loop the second way around, only with the stack one less
        //Too complex algorithm?


        

    
    

_exit:
    b .

.section .data
LI:
    .byte 1, 3, 4, 7, 9, 12
LI_LENGTH:
    .byte 6
D:
    .byte 11
DATA_INDEX:
	.word 0xE000
DATA_MATCHES:
    .word 0xF000
