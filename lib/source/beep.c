#include "beep.h"

void beepInit(){
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
}
void beepStop(){
	HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
}

void eatMusic(){
	for(uint8_t i = 0; i < 100; i++){
	HAL_Delay(1);
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, i);
	}
}
