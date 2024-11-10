#define DIGITRON_GLOBAL

#include "digitron.h"
#include "tim.h"
#include "string.h"

struct GPIO{
	GPIO_TypeDef* port;
	uint16_t pin;
};
#define NUM_DIGITRONS 8
static uint8_t digitron_buf[NUM_DIGITRONS];
/*-------------------定义数码管可显示的符号（8个单独的段）------------------------*/

static void sftClk(void);



/*----------------------实现74HC595的位选-------------------------*/
const struct GPIO HC595[3] = {
	{GPO_HC595_PB3_GPIO_Port, GPO_HC595_PB3_Pin},
	{GPO_HC595_PB4_GPIO_Port, GPO_HC595_PB4_Pin},
	{GPO_HC595_PB5_GPIO_Port, GPO_HC595_PB5_Pin},
};

const struct GPIO HC183[3] = {
	{GPO_HC183_PA0_GPIO_Port, GPO_HC183_PA0_Pin},
	{GPO_HC183_PA1_GPIO_Port, GPO_HC183_PA1_Pin},
	{GPO_HC183_PA2_GPIO_Port, GPO_HC183_PA2_Pin},
};


void digitronPutBuf(uint8_t numb, uint8_t section)
{
  digitron_buf[numb] = section;
}


void putData(uint8_t segments) {
    for(uint8_t i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(HC595[0].port, HC595[0].pin, (segments >> (7-i)) & 0x01);
        sftClk();
    }
}

//给PB5引脚一个上升沿
void sftClk(void){
	HAL_GPIO_WritePin(HC595[2].port, HC595[2].pin, RESET);
	HAL_GPIO_WritePin(HC595[2].port, HC595[2].pin, SET);
}

//给PB4引脚一个上升沿
void latchClk(void){
	HAL_GPIO_WritePin(HC595[1].port, HC595[1].pin, RESET);
	HAL_GPIO_WritePin(HC595[1].port, HC595[1].pin, SET);
}

/*----------------------实现数码管段选后显示任意一段-------------------------------*/
void chooseDigitron(uint8_t position){
	for(uint16_t i = 0; i < 3; i++){
		HAL_GPIO_WritePin(HC183[i].port, HC183[i].pin, (position >> i) & 0x01);
	}
}



/*----------------------更新显示到缓冲区----------------------*/
void displayToken(uint8_t n, uint8_t section) {
	HAL_Delay(0.5);
	putData(section);
	latchClk();
    chooseDigitron(n);

}
/*显示开始界面
 *
 */
void showStart(){
	 displayToken(0, 0x40);
	 displayToken(1, 0x40);
	 displayToken(2, 0x76);
	 displayToken(3, 0x79);
	 displayToken(4, 0x38);
	 displayToken(5, 0x38);
	 displayToken(6, 0x3f);
	 displayToken(7, 0x40);
}


/*显示暂停界面
 *
 */
void showPause(){
	 displayToken(2, 0x73);
	 displayToken(3, 0x77);
	 displayToken(4, 0x3E);
	 displayToken(5, 0x6D);
	 displayToken(6, 0x79);
}

/*显示Lose
 *
 */
void showEnd(){
	 displayToken(2, 0x38);
	 displayToken(3, 0x3f);
	 displayToken(4, 0x6D);
	 displayToken(5, 0x79);
}


