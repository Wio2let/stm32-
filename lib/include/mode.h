/*
 * mode.h
 *
 *  Created on: Nov 6, 2024
 *      Author: kurumi
 */

#ifndef MODE_H_
#define MODE_H_
#ifdef MODE_GLOBAL
	#define MODE_EXTERN
#else
	#define MODE_EXTERN extern
#endif
#include "main.h"
enum MODE{
	START,	RUNNING, PAUSE, END
};
enum Difficulty{
	EASY, NORMAL, HARD
};
MODE_EXTERN enum Difficulty difficulty;

#endif /* MODE_H_ */
