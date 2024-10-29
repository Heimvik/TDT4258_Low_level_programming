
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

#include <avr/io.h>

void enableAllPullups(){
    PORTA.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm;

    // Enable pull-ups for each specified pin on PORTB
    PORTB.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN2CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTB.PIN7CTRL |= PORT_PULLUPEN_bm;

    // Enable pull-ups for each specified pin on PORTC
    PORTC.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN2CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL |= PORT_PULLUPEN_bm;

    // Enable pull-ups for each specified pin on PORTD
    PORTD.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTD.PIN7CTRL |= PORT_PULLUPEN_bm;

    // Enable pull-ups for each specified pin on PORTE
    PORTE.PIN0CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN1CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN2CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTE.PIN7CTRL |= PORT_PULLUPEN_bm;

    // Enable pull-ups for each specified pin on PORTF
    PORTF.PIN2CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN3CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTF.PIN7CTRL |= PORT_PULLUPEN_bm;
}

void disableAllInputBuffers() {
    // Disable input buffers for each pin on PORTA
    PORTA.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTA.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable input buffers for each pin on PORTB
    PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTB.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable input buffers for each pin on PORTC
    PORTC.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTC.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable input buffers for each pin on PORTD
    PORTD.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable input buffers for each pin on PORTE
    PORTE.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTE.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Disable input buffers for each pin on PORTF
    PORTF.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTF.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTF.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTF.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTF.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTF.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;
}


void initAC(int mode){
    //PD2 as analog input
    PORTD.DIRCLR = PIN2_bm; 
    PORTD.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;

    
    //Set reference voltage to 1.024V for AC and DAC
    VREF.ACREF = VREF_REFSEL_1V024_gc;
    VREF.DAC0REF = VREF_REFSEL_1V024_gc;
    //Select the DAC reference for the AC
    AC0.DACREF = 0x18;
    
    //Set Control A register and let it aquire clock in standby mode
    AC0.CTRLA = AC_POWER_1_bm | AC_ENABLE_bm | AC_RUNSTDBY_bm;
    
    //Mux in AINP0 and DACREF to AC
    AC0.MUXCTRL = (0x1<<AC_INVERT_bp) | (0x0<<AC_MUXPOS_gp) | (0x3<<AC_MUXNEG_gp);
    
    AC0.INTCTRL |= (0x0<<AC_INTMODE_NORMAL_gp);
    
    switch(mode){
        case MODE_NORMAL:
            //Disable interrupts from AC (driven by STATUS)
            AC0.INTCTRL &= ~(0x1<<AC_CMP_bp);
            break;
        case MODE_AC_INTERRUPT:
            //Enable interrupts from AC
            AC0.INTCTRL |= (0x1<<AC_CMP_bp);
            break;
        case MODE_EVENT:
            //Disable interrupts from AC (driven by OUT)
            AC0.INTCTRL &= (0x1<<AC_CMP_bp);
            AC0.CTRLA |= AC_OUTEN_bm;
            break;
        default:
            break;
    }

    
}

uint8_t readAC(){
    return AC0.STATUS;
}

//Timer/Counter

void initTC(uint8_t ms_period){
    // Set the period of the timer. PER = period[s] * F_CPU / Prescaler = 0.01s * 4 000 000 Hz / 2
    TCA0.SINGLE.PER = (ms_period*1000)*F_CPU/PRESCALER;
    // Enable timer overflow interrupt
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    // Run timer in standby mode, set prescaler to 2, enable timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_RUNSTDBY_bm | TCA_SINGLE_CLKSEL_DIV2_gc | TCA_SINGLE_ENABLE_bm;
}

void initEvSys(){
    //AC0 as event generator for CHANNEL0
    EVSYS.CHANNEL0 = EVSYS_CHANNEL1_AC0_OUT_gc;
    //EVOUTA as event user for CHANNEL0
    EVSYS.USEREVSYSEVOUTA = EVSYS_USER_CHANNEL0_gc;
    PORTMUX.EVSYSROUTEA &= ~(0x1<<PORTMUX_EVOUTA_bp);
}

