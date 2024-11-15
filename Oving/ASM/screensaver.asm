.section .data
/*
Note:
All variables in the .data section IS automatically stored in memory and
can be loaded with the use of:
ldr r0, =red
ldr r0, [r0]
*/

//Spesifcs of the fb
fb_adr: .word 0xC8000000
uart_data_adr: .word 0xFF201000
uart_ctl_adr: .word 0xFF201004
//SS spesifics
block_start_adr: .word 0x1000

fb_px_x: .word 0x13F
fb_px_y: .word 0xEF

//Colors
black:      .hword 0x0000      // Black
white:      .hword 0xFFFF      // White
red:        .hword 0xF800      // Red
green:      .hword 0x07E0      // Green
blue:       .hword 0x001F      // Blue
yellow:     .hword 0xFFE0      // Yellow
cyan:       .hword 0x07FF      // Cyan
magenta:    .hword 0xF81F      // Magenta
gray:       .hword 0x7BEF      // Gray
orange:     .hword 0xFD20      // Orange
purple:     .hword 0x781F      // Purple
brown:      .hword 0x9A60      // Brown
light_gray: .hword 0xBDF7      // Light Gray
dark_gray:  .hword 0x39E7      // Dark Gray

//UART text
uart_test_str: .asciz "This is a test of the uart"


.section .text
.global _start

_start:
	b main

//Utility functionality

/*
Takes the input parameters:
r0: The adress of the first character to be written

Assummes null termination.
*/
writeStringToUart:
    push {lr}
    push {r4,r5}
    ldr r1, =uart_data_adr
    ldr r1, [r1]
    
    ldr r2, =uart_ctl_adr
    ldr r2, [r2]

    mov r3, #0
    ldr r4, =0xFFFF0000
    b writeCharacterToUart
    
    writeCharacterToUart:
        ldr r5, [r2]
        and r5, r4, r5
        cmp r5, #0
        beq writeCharacterToUart

        ldrb r5, [r0,r3]
        strb r5, [r1,#0]
        add r3,r3,#1
        cmp r5,#0
        bne writeCharacterToUart
    
    pop {r4,r5}
    pop {lr}

    bx lr


/*
Takes the input parameters:
r0: the adress of the first character

Stores the data in a null-terminated manner
*/
readStringFromUart:
    push {lr}
    push {r4,r5,r6,r7}

    ldr r1, =uart_data_adr
    ldr r1, [r1]
    
    ldr r2, =uart_ctl_adr
    ldr r2, [r2]

    mov r3, #0
    ldr r4, =0xFFFF0000
    b readCharFromUart

    readCharFromUart:
        ldr r5, [r1]
        and r6, r5, r4
        cmp r6, #0
        beq uartEmpty
        
        and r7,r5,#0xFF
        strb r7,[r0,r3]
        add r3,r3,#1
        b readCharFromUart
    
    uartEmpty:
        mov r5,#0
        strb r5,[r0,r3]
        
        pop {r4,r5,r6,r7}
        pop {lr}

        bx lr
/*	
Takes the input prameters:
r0: the x pixel
r1: the y pixel

Gives the output prarameters:
r0: the offset
*/
findOffset:
    push {lr}

    lsl r0, r0, #1
    lsl r1, r1, #10
    orr r0, r1, r0

    pop {lr}

    bx lr

/*
Takes the input parameters:
r0: x pixel
r1: y pixel
r2: the value of the color to set
*/
setPixel:
    push {lr}
    
    bl findOffset
    
    ldr r1, =fb_adr
    ldr r1, [r1]
    add r1, r0, r1
    strh r2, [r1]

    pop {pc}

/*
Takes the input parameters:
r0: The color value of the color to fill the screen with
*/
fillScreen:
    push {lr}
    push {r0}

    mov r0,#0
    mov r1,#0
    ldr r2, =fb_px_x
    ldr r2, [r2]
    ldr r3, =fb_px_y
    ldr r3, [r3]

    bl drawBlock

    add sp,sp,#4
    pop {pc}

/*
Takes the input parameters:
r0: The pointer to store it at
r1: The x position of upper left
r2: The y position of upper left
r3: The x width
S[0]-S[3]: The y height
S[4]-S[7]: The color of the block
*/
initBlock:
    push {lr}
    push {r4-r5}

    ldr r4,[sp,#12]
    ldr r5,[sp,#16]

    lsl r2,r2,#16
    orr r1,r1,r2
    str r1,[r0,#0]
    lsl r4,r4,#16
    orr r3,r3,r4
    str r3,[r0,#4]
    str r5,[r0,#8]

    pop {r4-r5}
    pop {pc}


/*
Takes the input parameters:
r0: Pointer to the current block, being stored *lowest adress* [posY(hword)posX(hword),widthY(hword)widthX(hword),color] *highest adress* consecutive in memory
*/
drawBlock:
    push {lr} //Push return adress
    push {r4-r7} //Push clobbered callee-saved registers
    mov r5,r0

    mov r0,#0
    mov r1,#0
    mov r2,#0
    mov r3,#0
    ldrh r0,[r5,#0]
    ldrh r1,[r5,#2]
    ldrh r2,[r5,#4]
    ldrh r3,[r5,#6]
    ldr r4,[r5,#8]

    mov r6, r2
    add r6, r6, r0
    mov r7, r3
    add r7, r7, r1
    mov r2, r4

    fillY:
        mov r0,#0
        ldrh r0,[r5,#0]
        fillX:
            push {r0,r1,r2,r3} //Push callee-saved registers
            bl setPixel
            pop {r0,r1,r2,r3} //Pop (restore) callee-saved registers

            add r0,r0,#1
            cmp r0,r6
            bne fillX
        add r1,r1,#1
        cmp r1,r7
        bne fillY

    pop {r4,r5,r6,r7}
    pop {pc} 

/*
Takes the input parameters:
r0: Pointer to the current block, being stored *lowest adress* [posY(hword)posX(hword),widthY(hword)widthX(hword),color] *highest adress* consecutive in memory
r1: Move px x direction
r2: Move px y direction
*/
moveBlock:
    push {lr}
    push {r4-r8}
    ldr r4,[r0,#0]
    ldr r7,=0xFFFF
    ldr r8,=0xFFFF0000
    and r5,r4,r7
    and r6,r4,r8
    
    add r5,r1,r5
    add r6,r2,r6

    orr r4,r5,r6
    str r4,[r0,#0]
 
    pop {r4-r8}
    pop {pc}

main:
    ldr r0, =block_start_adr
    ldr r0, [r0]
    push {r0}

    ldrh r0, =red
    mov r1,#0
    ldrh r0, [r0]
    push {r0}
    
    mov r4, #50
    push {r4}
    
    mov r1, #10
    mov r2, #20
    mov r3, #40

    ldr r0,[sp,#8]
    bl initBlock
    
    ldr r0,[sp,#8]
    bl drawBlock

    ldr r0,[sp,#8]
    mov r1,#100
    mov r2,#100
    bl moveBlock

    ldr r0,[sp,#8]
    bl drawBlock

    b .
