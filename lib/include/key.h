#ifndef __KEY_H
#define __KEY_H
#include "main.h"

#ifdef KEY_GLOBAL
	#define KEY_EXTERN
#else
	#define KEY_EXTERN extern
#endif

//定义按键状态
enum KEY{
	KEY_NULL, 	KEY_DOWN, 	KEY_UP, 	KEY_UP_SHORT, 	KEY_DOWN_SHORT,  KEY_LEFT_SHORT, 	KEY_RIGHT_SHORT,
	KEY_UP_LONG, 	KEY_DOWN_LONG,		KEY_LEFT_LONG,  	KEY_RIGHT_LONG
};

struct GPIO_KEY {
	GPIO_TypeDef* port;
	uint16_t pin;
};

#define KEY_LONG_THRESHOLD   80     /*长按键阈值*/
void keyInit(void);
enum KEY keyRead(void);
void keyResponse();





#endif
