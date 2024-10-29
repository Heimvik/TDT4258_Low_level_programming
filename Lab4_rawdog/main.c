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
#include "internals.h"

int main(void) {

    initLed();
    USART3_Init();

    // Send a message to USART indicating the LED is off initially
    USART3_SendString("LED is OFF\n");

    // Main loop
    while (1) {
        // Toggle LED state
        setLed();
        USART3_SendString("On");
        _delay_ms(1000);
        clearLed();
        USART3_SendString("Off");
        // Send LED status over USART
        clearLed();
    }
}
