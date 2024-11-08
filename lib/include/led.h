/*
 * led.h
 *
 *  Created on: Oct 29, 2024
 *      Author: kurumi
 */

#ifndef LED_H_
#define LED_H_
#include "main.h"

#define LED_0   0x01
#define LED_1   0x02
#define LED_2   0x04
#define LED_3   0x08
#define LED_4   0x10
#define LED_5   0x20
#define LED_6   0x40
#define LED_7   0x80
#define LED_ALL 0xFF

void ledOn(uint8_t val);
void ledOff(uint8_t val);
void ledToggle(uint8_t val);


#endif /* LED_H_ */
