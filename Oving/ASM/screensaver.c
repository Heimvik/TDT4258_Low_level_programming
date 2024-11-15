#include <stdint.h>

#define FB_ADRESS 0xC8000000
#define FB_PX_X 0x13F
#define FB_PX_Y 0xEF

#define BLOCK_START_ADRESS 0x1000

#define UART_DATA_ADR 0xFF201000
#define UART_CTL_ADR 0xFF201004

#define TIMER_BASE 0xFFFEC600
#define TIMER_LOAD 0x0
#define TIMER_VALUE 0x4
#define TIMER_CONTROL 0x8
#define TIMER_STATUS 0xC
#define TIMER_FREQUENCY 200000000
#define TIMER_ENABLE_BIT 0x0
#define TIMER_STATUS_BIT 0x2
#define TIMER_AUTO_BIT 0x1
#define TIMER_FLAG_BIT 0x0
#define TIMER_PRESCALER_BIT 8
#define END_TIME 11

void loadTimer();
void writeStringToUart(char* string);


void loadTimer(unsigned int frequency){
    unsigned int* timerCtrl = (unsigned int*)(TIMER_BASE+TIMER_CONTROL);
    *timerCtrl &= ~(1<<TIMER_ENABLE_BIT);
    *timerCtrl |= (1<<TIMER_AUTO_BIT);
    *timerCtrl |= (1<<TIMER_PRESCALER_BIT);

    unsigned int* timerLoad = (unsigned int*)(TIMER_BASE+TIMER_LOAD);
    *timerLoad = ((unsigned int)TIMER_FREQUENCY)/frequency;

    *timerCtrl |= (1<<TIMER_ENABLE_BIT);
}

//Now, your task is to make the asm functions usable in a c program, by calling the respective c fucntions
void writeStringToUart(char* string){
    __asm__(
        "push {lr}\n\t"
        "push {r4,r5,r6}\n\t"
        "mov r3, #0\n\t"
        "ldr r4, =0xFFFF0000\n\t"

        "writeCharacterToUart:\n\t"
        "ldr r5, [%[aUartCtrlAdr]]\n\t"
        "and r5, r4, r5\n\t"
        "cmp r5, #0\n\t"
        "beq writeCharacterToUart\n\t"

        "ldrb r5, [%[aString], r3]\n\t"
        "ldr r6, =%[aUartDataAdr]\n\t"
        "strb r5, [r6]\n\t"
        "add r3, r3, #1\n\t"
        "cmp r5, #0\n\t" 
        "bne writeCharacterToUart\n\t"

        "pop {r4, r5, r6}\n\t"
        "pop {lr}\n\t"
        "bx lr\n\t"

        : //No return value
        : [aString] "r" (string), [aUartDataAdr] "r" (UART_DATA_ADR), [aUartCtrlAdr] "r" (UART_CTL_ADR)
        : //No clobbered (handled myself)
    );
}

int main(){
    volatile char testString[] = "I love icecream";
    writeStringToUart(testString);
}