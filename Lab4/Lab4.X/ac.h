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
    void disableAllInputBuffers();
    void initAC(int mode);
    uint8_t readAC();
    
    void initTC(uint8_t ms_period);
    
    enum ACmode {
        MODE_NORMAL = 0,
        MODE_AC_INTERRUPT = 1,
        MODE_EVENT = 2
    };
    void initEvSys();

#ifdef	__cplusplus
}
#endif

#endif	/* INTERNALS_H */

