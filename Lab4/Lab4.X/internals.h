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
#define PD2_POS 2
#define PA2_POS 2
#define INVEN_POS 7    

    
    void initAnalogComparator(); 
    void initLed();
    void setLed();
    void clearLed();


#ifdef	__cplusplus
}
#endif

#endif	/* INTERNALS_H */

