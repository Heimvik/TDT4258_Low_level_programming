#include "led.h"

void initLed() {
    PORTA.DIRSET = PIN2_bm;
    PORTA.PIN2CTRL |= PORT_PINCTRLSET_7_bm;
}
void setLed(){
    // LED is active low. Set pin LOW to turn LED on
    PORTA.OUTSET |= PIN2_bm;
}
void clearLed(){
    // LED is active low. Set pin HIGH to turn LED off
    PORTA.OUTCLR |= PIN2_bm;
}
