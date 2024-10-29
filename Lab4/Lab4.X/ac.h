/* 
 * File:   internals.h
 * Author: christian-heimvik
 *
 * Created on October 29, 2024, 11:40 AM
 */

#ifndef INTERNALS_H
#define	INTERNALS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <avr/io.h>
#include "usart.h"
    
#define PRESCALER 2

    void printACRegisters();
    
    void initAC(uint8_t interruptDriven);
    uint8_t readAC();
    
    void initTC(uint8_t ms_period);
    


#ifdef	__cplusplus
}
#endif

#endif	/* INTERNALS_H */

