#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <stdint.h>    
#include <stdbool.h>   
#include <limits.h>
#include <unistd.h>


#define SEG7_DATA_0_ADR 0xFF200020
#define SEG7_DATA_1_ADR 0xFF200030
#define SEG7_DIGIT_0 0b00111111
#define SEG7_DIGIT_1 0b00000110
#define SEG7_DIGIT_2 0b01011011
#define SEG7_DIGIT_3 0b01001111
#define SEG7_DIGIT_4 0b01100110
#define SEG7_DIGIT_5 0b01101101
#define SEG7_DIGIT_6 0b01111101
#define SEG7_DIGIT_7 0b00000111
#define SEG7_DIGIT_8 0b01111111
#define SEG7_DIGIT_9 0b01101111

#define LED_ADR 0xFF200000
#define PB_ADR 0xFF200050

#define UART_DATA_ADR 0xFF201000
#define UART_CONTROL_ADR 0xFF201004

const uint8_t seg7Digits[10] = {(uint8_t)SEG7_DIGIT_0,(uint8_t)SEG7_DIGIT_1,(uint8_t)SEG7_DIGIT_2,(uint8_t)SEG7_DIGIT_3,(uint8_t)SEG7_DIGIT_4,(uint8_t)SEG7_DIGIT_5,(uint8_t)SEG7_DIGIT_6,(uint8_t)SEG7_DIGIT_7,(uint8_t)SEG7_DIGIT_8,(uint8_t)SEG7_DIGIT_9};

void setDigitSeg7C(uint32_t digit,uint32_t position);
void setDigitSeg7Asm(uint8_t digit,uint8_t* digitMaskArray, uint32_t position,uint32_t seg7Data0Adress,uint32_t seg7Data1Adress);
void connectButtonsWithLedsAsm(uint8_t pbToPoll, uint32_t* pbAdress, uint8_t ledToSet,uint32_t* ledAdress,uint8_t mode);


/*
The other alternative, using the asm() and procedure call manually
*/
asm(
    "setDigitSeg7Asm:\n\t"
    "push {lr}\n\t"
    "push {r4-r6}\n\t"//r4 on the lowest memory adress,r6 at the highest
    "ldr r4, [sp,#16]\n\t"
    "ldrb r1, [r1,r0]\n\t"
    "and r1,#0xFF\n\t"

    "cmp r2,#4\n\t"
    "ble writeSeg7\n\t"

    "mov r3,r4\n\t"
    "sub r2,r2,#4\n\t"
    "b writeSeg7\n\t"

    "writeSeg7:\n\t"
        "ldr r5,[r3]\n\t"
        "mov r0,#0xFF\n\t"
        "mov r6,#8\n\t"
        "mul r6,r6,r2\n\t"
        "lsl r0,r0,r6\n\t"
        "mvn r0,r0\n\t"
        "and r5,r5,r0\n\t"
        "orr r5,r5,r1\n\t"
        "str r5,[r3]\n\t"
    "pop {r4-r6}\n\t"
    "pop {pc}\n\t"
);

/*
Make a c function that does the same
*/
void setDigitSeg7C(uint32_t digit, uint32_t position) {
    volatile uint32_t* seg7Data;
    if(position<=3){
        seg7Data = (uint32_t*)(SEG7_DATA_0_ADR);
    } else {
        position = position - 4;
        seg7Data = (uint32_t*)(SEG7_DATA_1_ADR);
    }
    seg7Data[position] = seg7Digits[digit];
}

/*
Make a function in assembly that takes in:
- The PB to check
- The base adress of the switches
- The LED to set
- The base adress of the leds
- To set, clear or thoggle the leds
*/
asm(
    "connectButtonsWithLedsAsm:"
        "push {lr}\n\t"
        "push {r4-r8}\n\t" //Puts r4 at the lowst adress, r6 at the highest
        "ldr r4,[sp,#24]\n\t"
        "ldr r5,[r1]\n\t"
        "and r5,#0xF\n\t" //Removes higher order bits if any
        "mov r6,#0b1\n\t"
        "mov r7,#0\n\t"
        "b loopButtons\n\t"
        "loopButtons:\n\t"
            "and r1,r5,r6\n\t"
            "orr r7,r7,r1\n\t"
            "lsl r6,#1\n\t"
            "mov r8,#0x10\n\t"
            "cmp r6,r8\n\t"
            "bne loopButtons\n\t"
        "mov r8, #0x1\n\t"
        "cmp r4, r8\n\t"
        "blt thoggleLeds\n\t"
        "beq setLeds\n\t"
        "bgt clearLeds\n\t"

        "thoggleLeds:\n\t"
            "ldr r0,[r3]\n\t"
            "eor r0,r0,r7\n\t"
            "str r0,[r3]\n\t"
            "b cleanup\n\t"
        "setLeds:\n\t"
            "mov r0,r7\n\t"
            "str r0,[r3]\n\t"
            "b cleanup\n\t"
        "clearLeds:\n\t"
            "ldr r0,[r3]\n\t"
            "mvn r7,r7\n\t"
            "and r0,r0,r7\n\t"
            "str r0,[r3]\n\t"
            "b cleanup\n\t"
        "cleanup:\n\t"
            "pop {r4-r8}\n\t"
            "pop {pc}\n\t"
);


void writeToUart(char* stringToWrite,uint32_t* uart_data, uint32_t* uart_control);
/*
Has the parameters:
- r0 the pointer to the first char to write
- r1 uart data register
- r2 uart control register
*/
asm(
    "writeToUart:\n\t"
        "push {lr}\n\t"
        "push {r4,r5,r6}\n\t"
        "mov r6,#0x0\n\t"//const 0 for cmp
        "mov r7,#0x0\n\t"//Iterator on chars
        "b loopChars\n\t"
        "loopChars:\n\t"
            "ldrb r3,[r0,r7]\n\t"
            "cmp r3,r6\n\t"
            "beq cleanUpWriteUart\n\t"
            "b checkSpace\n\t"
            "checkSpace:\n\t"
                "ldr r4, =0xFFFF0000\n\t"
                "ldr r5,[r2]\n\t"
                "and r4,r5\n\t"
                "cmp r4,r6\n\t"
                "beq checkSpace\n\t"
                "strb r3,[r1]\n\t"
                "add r7,r7,#1\n\t"
                "b loopChars\n\t"
        "cleanUpWriteUart:\n\t"
            "pop {r4,r5,r6}\n\t"
            "pop {pc}\n\t"
);

uint8_t readFromUart(char* dest,uint32_t* uart_data, uint32_t* uart_control);
asm(
    "readFromUart:\n\t"
        "push {lr}\n\t"
        "push {r4-r7}\n\t"
        
        "mov r6,#0x0\n\t"//Iterator for the current char we are placing
        "mov r7,#0\n\t"//constant zero value to cmp with
        "b readNextChar\n\t"
        "readNextChar:\n\t"
            "ldr r3,[r1]\n\t"
            "mov r4,#0b1\n\t"
            "lsl r4,r4,#15\n\t"
            "and r4,r4,r3\n\t"
            "cmp r4,r7\n\t"
            "beq finishedReading\n\t"
            "and r3,r3,#0xFF\n\t"
            "strb r3,[r0,r6]\n\t"
            "add r6, r6, #1\n\t"
            "b readNextChar\n\t"
        "finishedReading:\n\t"
            "strb r7,[r0,r6]\n\t"
            "mov r0,r6\n\t"
            "pop {r4-r7}\n\t"
            "pop {pc}\n\t"
);


int main(){
    //setDigitSeg7Asm((uint32_t)3,seg7Digits,(uint32_t)0,(uint32_t)SEG7_DATA_0_ADR,(uint32_t)SEG7_DATA_1_ADR);
    char* testString = "Initial stringxxxxxxxxxxxxxxxx";
    while(1){
        if (readFromUart(testString,(uint32_t*)UART_DATA_ADR,(uint32_t*)UART_CONTROL_ADR) != 0) {
            writeToUart(testString,(uint32_t*)UART_DATA_ADR,(uint32_t*)UART_CONTROL_ADR);
        }
    }
    while(1){
        connectButtonsWithLedsAsm(0,(uint32_t*)PB_ADR,0,(uint32_t*)LED_ADR,1);
    }
    return 0;
}