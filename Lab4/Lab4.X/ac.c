
#include "ac.h"



// Analog comparator

void printACRegisters() {
    // Print each register with a label
    USART3_SendHex(PORTD.DIR, "PORTD.DIR");
    USART3_SendHex(PORTD.PIN2CTRL, "PORTD.PIN2CTRL");
    USART3_SendHex(VREF.ACREF, "VREF.ACREF");
    USART3_SendHex(VREF.DAC0REF, "VREF.DAC0REF");
    USART3_SendHex(AC0.DACREF, "AC0.DACREF");
    USART3_SendHex(AC0.CTRLA, "AC0.CTRLA");
    USART3_SendHex(AC0.MUXCTRL, "AC0.MUXCTRL");
    USART3_SendHex(AC0.INTCTRL, "AC0.INTCTRL");
}

void initAC(uint8_t interruptDriven){
    //PD2 as analog input
    PORTD.DIRCLR = PIN2_bm; 
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;

    
    //Set reference voltage to 1.024V for AC and DAC
    VREF.ACREF = VREF_REFSEL_1V024_gc;
    VREF.DAC0REF = VREF_REFSEL_1V024_gc;
    //Select the DAC reference for the AC
    AC0.DACREF = 0x18;
    
    //Set Control A register
    AC0.CTRLA = AC_ENABLE_bm; //| AC_RUNSTDBY_bm;
    
    //Mux in AINP0 and DACREF to AC
    AC0.MUXCTRL = (0x1<<AC_INVERT_bp) | (0x0<<AC_MUXPOS_gp) | (0x3<<AC_MUXNEG_gp);
    
    AC0.INTCTRL |= (0x0<<AC_INTMODE_NORMAL_gp);
    
    if(interruptDriven){
        AC0.INTCTRL |= (0x1<<AC_CMP_bp);
    } else {
        AC0.INTCTRL &= ~(0x1<<AC_CMP_bp);
    }

    
}

uint8_t readAC(){
    return AC0.STATUS;
}


//Timer/Counter
void initTC(uint8_t ms_period){
    // Set the period of the timer. PER = period[s] * F_CPU / Prescaler = 0.01s * 4 000 000 Hz / 2
    TCA0.SINGLE.PER = 20000;
    // Enable timer overflow interrupt
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    // Run timer in standby mode, set prescaler to 2, enable timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_RUNSTDBY_bm | TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
}

ISR(TCA0_OVF_vect)
{
    // Add your code here 
    
    // Clear interrupt flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
