
#include "internals.h"

void initAnalogComparator(uint8_t interruptDriven){
    //PD2 as analog input
    PORTD.DIRCLR &= ~(0x1<< PD2_POS);
    //Mux in AINP0 and DACREF to AC
    AC0.MUXCTRL =(0x1<<AC_MUXCTRL_INVERT_POS)&(0x0<<AC_MUXCTRL_P_POS)&(0x3<<AC_MUXCTRL_N_POS);
    
    //Set reference voltage divider to (16+64/256)*3.3
    AC0.DACREF = 0x50;
    
    //Set Control A register
    AC0.CTRLA |= (0x1<<AC_CTRLA_RUNSTDBY_POS)&(0x1<<AC_CTRLA_EN_POS);
    
    if(interruptDriven){
        AC0.INTCTRL |= (0x0<<AC_INTCTRL_INTMODE_POS) & (0x1<<AC_INTCTRL_CMP_POS);
    } else {
        AC0.INTCTRL &= (0x0<<AC_INTCTRL_CMP_POS);
    }

    //Set outputs:
    //CMP?
    
}

uint8_t readAnalogComparator(){
    return AC0.STATUS;
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