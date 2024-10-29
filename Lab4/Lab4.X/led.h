/* 
 * File:   led.h
 * Author: christian-heimvik
 *
 * Created on October 29, 2024, 7:14 PM
 */
#include <avr/io.h>
#include "usart.h"

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

void initLed();
void setLed();
void clearLed();

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

