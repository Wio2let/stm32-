/*
 * snake.c
 *
 *  Created on: Oct 29, 2024
 *      Author: kurumi
 */
#define SNAKE_GLOBAL
#include "snake.h"




uint16_t moveOneStep = 0;
uint8_t foodGenFlag = 0;
uint8_t foodDispFlag = 0;
uint8_t repeatFlag = 0;
static uint8_t randNum = 0;
static uint8_t randNumSec = 0;
static Point2D coordinate[48] = {{0}};
typedef struct {
    uint8_t newIndex;  // 新的移动方向
    uint8_t nextSegment;    // 下一个段的值
} DirectionTransition;


const Position startPos = {
	.n = 1,  // 最左边的数码管
	.section = 0x01 // a段
};
static Direction updatedDir = RIGHT;

SNAKE_EXTERN  struct SNAKE_STATUS* mainSnake = NULL; // 初始化为 NULL
SNAKE_EXTERN  struct SNAKE_STATUS* copySnake = NULL;
//SNAKE_EXTERN  struct SNAKE_STATUS* trueLastSnake = NULL;
// 定义蛇位置转换表
static const DirectionTransition TRANSITION_MAP[7][4][4] = {
    // a段(0x01)
    [0] = {
        // 当前向右移动时
        [RIGHT] = {
        	[RIGHT] = {1, 0x01},
            [UP] = {0, 0x01},      // 按上：无效，保持原位
            [DOWN] = {0, 0x02},   // 按下：转向下，到b段
        },
        // 当前向左移动时
        [LEFT] = {
        	[LEFT] = {7, 0x01},
            [UP] = {0, 0x01},      // 按上：无效，保持原位
            [DOWN] = {0, 0x20},   // 按下：转向下，到f段
        },
    },
    // b段(0x02)
    [1] = {
        [UP] = {
        	[UP] = {0, 0x02},
            [LEFT] = {0, 0x01},   // 按左：转向左，到a段
            [RIGHT] = {1, 0x01},  // 按右：转向右，到a段
        },
        [DOWN] = {
        	[DOWN] = {0, 0x04},
            [LEFT] = {0, 0x40},   // 按左：转向左，到g段
            [RIGHT] = {1, 0x40},  // 按右：转向右，到g段
        },
    },
    // c段(0x04)
    [2] = {
        [UP] = {
        	[UP] = {0, 0x02},
            [LEFT] = {0, 0x40},   // 按左：转向左，到g段
            [RIGHT] = {1, 0x40}  // 按右：转向右，到g段
        },
        [DOWN] = {
        	[DOWN] = {0, 0x04},
            [LEFT] = {0, 0x08},   // 按左：转向左，到d段
            [RIGHT] = {1, 0x08}  // 按右：转向右，到d段
        }
    },
    // d段(0x08)
    [3] = {
        [RIGHT] = {
        	[RIGHT] = {1, 0X08},
            [UP] = {0, 0x04},      // 按上：转向上，到c段
            [DOWN] = {0, 0x08},   // 按下：无效，保持原位
        },
        [LEFT] = {
        	[LEFT] =  {7, 0x08},
            [UP] = {0, 0x10},      // 按上：转向上，到e段
            [DOWN] = {0, 0x08},   // 按下：无效，保持原位
        }
    },
    // e段(0x10)
    [4] = {
        [UP] = {
        	[UP] = {0, 0x20},
            [LEFT] = {7, 0x40},   // 按左：转向左, 到g段
            [RIGHT] = {0, 0x40}  // 按右：转向右，到g段
        },
        [DOWN] = {
        	[DOWN] = {0, 0x10},
            [LEFT] = {7, 0x08},   // 按左：转向左，到d段
            [RIGHT] = {0, 0x08}  // 按右：转向右，到d段
        }
    },
    // f段(0x20)
    [5] = {
        [UP] = {
        	[UP] = {0, 0x20},
            [LEFT] = {7, 0x01},   // 按左：转向左，到a段
            [RIGHT] = {0, 0x01}  // 按右：转向右，到a段
        },
        [DOWN] = {
        	[DOWN] = {0, 0x10},
            [LEFT] = {7, 0x40},   // 按左：转向左，到g段
            [RIGHT] = {0, 0x40}  // 按右：转向右，到g段
        }
    },
    // g段(0x40)
    [6] = {
        [RIGHT] = {
        	[RIGHT] = {1, 0x40},
            [UP] = {0, 0x02},      // 按上：转向上，到b段
            [DOWN] = {0, 0x04},   // 按下：转向下，到c段
        },
        [LEFT] = {
        	[LEFT] = {7, 0x40},
            [UP] = {0, 0x20},      // 按上：转向上，到f段
            [DOWN] = {0, 0x10},   // 按下：转向下，到e段
        }
    }
};

/*--------------创建蛇的每个块----------------*/
struct SNAKE_STATUS *createSnakeNode(Position pos, Direction dir){
	struct SNAKE_STATUS* headNode = (struct SNAKE_STATUS *)malloc(sizeof(struct SNAKE_STATUS));
	if(headNode == NULL){
		return NULL;
	}
	headNode->position.n = pos.n;
	headNode->position.section = pos.section;
	headNode->towards = dir;
	headNode->next = NULL;
	return headNode;
}

/*---------在蛇尾添加新的块-------------*/
struct SNAKE_STATUS *addBlock(struct SNAKE_STATUS* head, struct SNAKE_STATUS* tail){

	struct SNAKE_STATUS* currentNode = head;
	//找到最后的节点
	while(currentNode->next != NULL){
		currentNode = currentNode->next;
	}
	struct SNAKE_STATUS* bodyNode = createSnakeNode(tail->position, tail->towards);
	currentNode->next = bodyNode;
	return head;
}


/*---------记录蛇是否动了一步---------*/
void recordStep(void){
	moveOneStep = !moveOneStep;
}

/*--------------------初始化显示蛇的每个块----------------------------*/
void snakeDisplay(struct SNAKE_STATUS* snake){
	struct SNAKE_STATUS* currentBlock = snake;  // 使用一个临时指针遍历
	do {
		displayToken(currentBlock->position.n, currentBlock->position.section);
		currentBlock = currentBlock->next;
	} while (currentBlock != NULL);

}
/*-------------------初始化蛇在左上角且长度为一-------------------*/
void snakeInit(void) {
    if (mainSnake == NULL) {
        mainSnake = createSnakeNode(startPos, RIGHT); // 仅在第一次调用时创建
        copySnake = createSnakeNode(startPos, RIGHT);

    }
}


/*-------------------更新每个块的移动-----------------------------*/
void snakeBlockUpdate(struct SNAKE_STATUS* snake){
	struct SNAKE_STATUS* currentBlock = snake;  // 使用一个临时指针遍历
	uint16_t i = 0;
	do{


		displayToken(currentBlock->position.n, currentBlock->position.section);
		coordinate[i] = get_segment_coordinates(currentBlock->position.n, currentBlock->position.section, currentBlock->towards);
		currentBlock = currentBlock->next;
		i++;
		//HAL_Delay(0.1);
	} while (currentBlock != NULL);

}


/*------------更改按下按键后蛇头的朝向--------------------*/
void snakeDirectionUpdated(struct SNAKE_STATUS* snake) {
	struct SNAKE_STATUS* currentBlock = snake;  // 使用一个临时指针遍历
	enum KEY status = keyRead();
	switch(status){
	case KEY_UP_SHORT:
		if(currentBlock->position.section == 0x40 || currentBlock->position.section == 0x08){
		currentBlock->towards = (currentBlock->towards==LEFT || currentBlock->towards == RIGHT) ? UP : currentBlock->towards;
		}
		break;
	case KEY_DOWN_SHORT:
		if(currentBlock->position.section == 0x40 || currentBlock->position.section == 0x01){
		currentBlock->towards = (currentBlock->towards==LEFT || currentBlock->towards == RIGHT) ? DOWN : currentBlock->towards;
		}
		break;
	case KEY_LEFT_SHORT:
		if((currentBlock->position.section & 0x36) != 0){
		currentBlock->towards = (currentBlock->towards==UP || currentBlock->towards == DOWN) ? LEFT : currentBlock->towards;
		}
		break;
	case KEY_RIGHT_SHORT:
		if((currentBlock->position.section & 0x36) != 0){
		currentBlock->towards = (currentBlock->towards==UP || currentBlock->towards == DOWN) ? RIGHT : currentBlock->towards;
		}
		break;
	default:

		break;
	};

}



/*------------更改按下按键后蛇头和所有段的位置--------------------*/
void snakePositionUpdated(struct SNAKE_STATUS* snake_main, struct SNAKE_STATUS* snake_copy) {
	struct SNAKE_STATUS* currentBlock = snake_main;
	struct SNAKE_STATUS* lastBlock = snake_copy;
	Direction lastDic = lastBlock->towards;
	updatedDir = currentBlock->towards;
	currentBlock->position.n = (TRANSITION_MAP[(uint8_t)log2(currentBlock->position.section)][lastDic][updatedDir].newIndex+currentBlock->position.n)%8;
	currentBlock->position.section = TRANSITION_MAP[(uint8_t)log2(currentBlock->position.section)][lastDic][updatedDir].nextSegment;
    while(currentBlock->next != NULL){
    	currentBlock = currentBlock->next;
    	currentBlock->position = lastBlock->position;
    	lastBlock =  lastBlock->next;
    }
}



/*------------重置copySnake为mainSnake-------------*/
void copySnakeReset(struct SNAKE_STATUS* snake_main, struct SNAKE_STATUS* snake_copy){
	struct SNAKE_STATUS* currentBlock = snake_main;
	struct SNAKE_STATUS* lastBlock = snake_copy;
	while (currentBlock != NULL) {
		lastBlock->towards = currentBlock->towards;
		lastBlock->position = currentBlock->position;
		if (currentBlock->next != NULL && lastBlock->next != NULL) {
			lastBlock = lastBlock->next;
		}
		else if(currentBlock->next != NULL && lastBlock->next == NULL)
		{
			lastBlock->next = createSnakeNode(currentBlock->next->position, currentBlock->next->towards);
			break;
		}
		currentBlock = currentBlock->next;

	}
}

/*----------随机生成食物的位置----------------------*/
void foodGen(struct SNAKE_STATUS* snake_main){
	struct SNAKE_STATUS* currentBlock = snake_main;
	do{
	repeatFlag = 0;
	randNum = HAL_GetTick() % 8;
	randNumSec =  HAL_GetTick() % 7;
	while(currentBlock != NULL){
		if(currentBlock->position.n == randNum && (0x01 << randNumSec) == currentBlock->position.section ){
			repeatFlag = 1;
			break;
		}
		currentBlock = currentBlock->next;
	}

	}while(repeatFlag);
	foodGenFlag = 1;
	foodDispFlag = 0;

};

/*--------------显示食物的位置--------------------*/
void foodDisplay(void){
	if(!foodDispFlag){
	displayToken(randNum, 0x01<<randNumSec);
	}

}

/*---------------判断食物是否被吃掉并长度加一-------------*/
void ifAteFood(struct SNAKE_STATUS* snake_main, struct SNAKE_STATUS* snake_copy){
	struct SNAKE_STATUS* currentBlock = snake_main;
	struct SNAKE_STATUS* lastBlock = snake_copy;

	if(randNum==currentBlock->position.n && ((0x01 <<randNumSec) == currentBlock->position.section) ){
		foodGenFlag = 0;
		foodDispFlag = 1;
		while(lastBlock->next!= NULL){
			lastBlock = lastBlock->next;
		}
		addBlock(snake_main, lastBlock);

	}

}



