/*
 * led.c
 *
 *  Created on: Oct 29, 2024
 *      Author: kurumi
 */

#include "led.h"

struct GPIO_LED{
	GPIO_TypeDef* port;
	uint16_t pin;
};
uint8_t score = 0;
const struct GPIO_LED led[] = {
	{GPO_LED_0_GPIO_Port, GPO_LED_0_Pin},
	{GPO_LED_1_GPIO_Port, GPO_LED_1_Pin},
	{GPO_LED_2_GPIO_Port, GPO_LED_2_Pin},
	{GPO_LED_3_GPIO_Port, GPO_LED_3_Pin},
	{GPO_LED_4_GPIO_Port, GPO_LED_4_Pin},
	{GPO_LED_5_GPIO_Port, GPO_LED_5_Pin},
	{GPO_LED_6_GPIO_Port, GPO_LED_6_Pin},
	{GPO_LED_7_GPIO_Port, GPO_LED_7_Pin},
};

#define NUM_LEDS sizeof(led) / sizeof(struct GPIO_LED)


void ledOn(uint8_t val){
	for(uint8_t i = 0; i < NUM_LEDS; i++){
		if(val & (0x01 << i))
			{HAL_GPIO_WritePin(led[i].port, led[i].pin, RESET);}
	}


}

void ledOff(uint8_t val){
	for(uint8_t i = 0; i < NUM_LEDS; i++){
			if(val & (0x01 << i))
				{HAL_GPIO_WritePin(led[i].port, led[i].pin, SET);}
		}

}


void ledToggle(uint8_t val){
	for(uint8_t i = 0; i < NUM_LEDS; i++){
				if(val & (0x01 << i))
					{HAL_GPIO_TogglePin(led[i].port, led[i].pin);}
			}
}

void ledScore(uint8_t score){
	ledOff(score-1);
	ledOn(score);
}
