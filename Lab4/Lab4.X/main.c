/* 
 * File:   main.c
 * Author: christian-heimvik
 *
 * Created on October 29, 2024, 12:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ac.h"
#include "led.h"




int main(void) {
    initAC(0);
    initLed();
    USART3_Init();

    USART3_SendString("All regs:\n");
    printACRegisters(); 
    // Main loop
    while (1) {
        USART3_SendHex(AC0.STATUS,"Status: ");
        
        if(readAC() & 0x10){
            setLed();
        } else {
            USART3_SendString("CLR\n");
            clearLed();
        }
        _delay_ms(100);
    }
}
