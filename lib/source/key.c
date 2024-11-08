#define KEY_GLOBAL
#include "main.h"
#include "tim.h"



const struct GPIO_KEY keyPort[] = {
		{GPI_KEY_UP_GPIO_Port, GPI_KEY_UP_Pin},
		{GPI_KEY_DOWN_GPIO_Port, GPI_KEY_DOWN_Pin},
		{GPI_KEY_LEFT_GPIO_Port, GPI_KEY_LEFT_Pin},
		{GPI_KEY_RIGHT_GPIO_Port, GPI_KEY_RIGHT_Pin}
};

#define NUM_KEYS sizeof(keyPort) / sizeof(struct GPIO_KEY)

//initialize key status
static enum KEY key = KEY_NULL;


/*----------初始化按键----------------*/

void keyInit(void){
	HAL_TIM_Base_Start_IT(&htim2);

}

/*---------读取按键值---------------*/
enum KEY keyRead(){
	enum KEY k;
	if(key >= KEY_UP_SHORT)
	{
		k = key;
	}
	else
		{
			k = key;
		}
	return k;
}

/*---------按键响应函数------------*/
void keyResponse(){
	static uint32_t currentTic = 0;
	static uint32_t lastTic = 0;
	static uint8_t keyIndex = 0;
	currentTic++;
	switch(key){
	case KEY_NULL:
		for(uint8_t i = 0; i<NUM_KEYS; i++){
			if(i != 0)
				{
				if(HAL_GPIO_ReadPin(keyPort[i].port, keyPort[i].pin) == 0)
					{
						key = KEY_DOWN;		//记录按键被按下
						lastTic = currentTic;
						break;
					}
				}
			else{
				if(HAL_GPIO_ReadPin(keyPort[i].port, keyPort[i].pin) == 1)
					{
						key = KEY_DOWN;		//按键Up按下是为1
						lastTic = currentTic;
						break;
					}
			}
		}
		break;
	case KEY_DOWN:
		for(uint8_t i = 0; i< NUM_KEYS; i++){
			if(i != 0){
				if(HAL_GPIO_ReadPin(keyPort[i].port, keyPort[i].pin) == 0)
				{
					keyIndex = i;	//按键未抬起，记录该键值
					break;
				}
			}
			else{
				if(HAL_GPIO_ReadPin(keyPort[i].port, keyPort[i].pin) == 1)
				{
					keyIndex = i;	//按键未抬起，记录该键值
					break;
				}

			}
			if(i >= NUM_KEYS-1)
			{
				key = KEY_UP;	//遍历完4个按键后发现没有按下的按键后切换状态

			}
		}
		break;
	case KEY_UP:
		if((currentTic - lastTic) > KEY_LONG_THRESHOLD){
			key = (enum KEY) (keyIndex+KEY_UP_LONG);		//记录长按键状态

		}
		else{
			key = (enum KEY) (keyIndex+KEY_UP_SHORT);		//记录短按键状态

		}
		break;
	default:
		if(!moveOneStep){

			break;
		}
		recordStep();
		key = KEY_NULL;


}
}



