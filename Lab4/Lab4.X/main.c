/* 
 * File:   main.c
 * Author: christian-heimvik
 *
 * Created on October 29, 2024, 12:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ac.h"
#include "led.h"


ISR(TCA0_OVF_vect)
{
    //Check the light in the room
    if(readAC() & 0x10){
        setLed();
    } else {
        clearLed();
    }
    
    // Clear interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
ISR(AC0_AC_vect)
{
    //Check the light in the room
    if(readAC() & 0x10){
        setLed();
    } else {
        clearLed();
    }
    
    // Clear interrupt flag
    AC0.STATUS |= (0x1<<AC_CMPIF_bp);
}



int main(void) {
    USART3_Init();
    initAC(1);
    initLed();
    

    //initTC(100);
    //Enable global interrupts
    sei();
    //Setup interrupt controller
    set_sleep_mode(SLEEP_MODE_STANDBY);
    while(1) {
        sleep_mode();
    }
    return 0;
}
