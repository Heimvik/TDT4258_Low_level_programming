
#include "internals.h"

void initAnalogComparator(){
    //Configure pins as analog input (PD2)
    PORTD.DIRCLR &= ~(0x1<< PD2_POS);
    
    
}

void initLed() {
    PORTA.DIRSET = PIN2_bm;
    PORTA.PIN2CTRL |= (0x1<<INVEN_POS);
}
void setLed(){
    // LED is active low. Set pin LOW to turn LED on
    PORTA.OUTSET |= PIN2_bm;
}
void clearLed(){
    // LED is active low. Set pin HIGH to turn LED off
    PORTA.OUTCLR |= PIN2_bm;
}