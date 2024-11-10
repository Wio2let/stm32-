/*
 * snake.h
 *
 *  Created on: Oct 29, 2024
 *      Author: kurumi
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include "main.h"
#include "tim.h"
#include "stdlib.h"
#include "math.h"
#include "shared.h"
#ifdef SNAKE_GLOBAL
	#define SNAKE_EXTERN
#else
	#define SNAKE_EXTERN extern
#endif



extern uint16_t moveOneStep;
extern uint8_t foodGenFlag;
struct SNAKE_STATUS {
	Position position;
	Direction towards;
	struct SNAKE_STATUS *next;

};
SNAKE_EXTERN struct SNAKE_STATUS* mainSnake;
SNAKE_EXTERN struct SNAKE_STATUS* copySnake;
SNAKE_EXTERN struct SNAKE_STATUS* trueLastSnake;
struct SNAKE_STATUS *createSnakeNode(Position , Direction );
struct SNAKE_STATUS *addBlock(struct SNAKE_STATUS* , struct SNAKE_STATUS*);
void recordStep(void);
void snakeDirectionUpdated(struct SNAKE_STATUS*);
void snakeDisplay(struct SNAKE_STATUS*);
void snakeBlockUpdate(struct SNAKE_STATUS*);

void snakeInit();

void snakePositionUpdated(struct SNAKE_STATUS*, struct SNAKE_STATUS*);
void copySnakeReset(struct SNAKE_STATUS*, struct SNAKE_STATUS*);
void foodGen(struct SNAKE_STATUS*);
void foodDisplay(void);
void ifAteFood(struct SNAKE_STATUS*, struct SNAKE_STATUS*);
void ifHitWall(struct SNAKE_STATUS* snake_main);
void ifHitSelf(struct SNAKE_STATUS* snake_main);
#endif /* SNAKE_H_ */
