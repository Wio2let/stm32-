# STM32F411RCT6——PDCA

November8, 2024

Kulumio

<aside>
🔖 PDCA报告——记一次嵌入式实习

</aside>

# **1.PLAN**

| 新建项目 | 配置引脚 | 配置时钟、中断 | 添加头文件夹、源文件夹路径 |
| --- | --- | --- | --- |
| 编写LED驱动 | 用LED灯亮的个数表示得分 | led二进制显示加分动画 | 游戏结束led清零 |
| 编写数码管驱动 | 动态显示蛇的位置 | 随机生成食物 | 显示加速 |
| 编写按键驱动 | 长短按键区分 | 4个短按键控制蛇的移动 | 4个长按键移动加速 |
| 编写蜂鸣器驱动 | 用pwm控制蜂鸣器声音 | 吃到食物后蜂鸣器响一下 | 游戏结算音乐 |
| 编写贪吃蛇整体框架 | 链表构建蛇 | 蛇的增长逻辑 | 蛇的碰撞检测 |
| 自定义UART通讯协议 | 切换UI界面 | wasd控制蛇的移动 | 难度选择 |

# 2.DO

## 准备工作

- 新建工程项目
    
    1.创建STM32工程
    
    ![image.png](/images/image.png)
    
    1. 选择对应芯片型号
    
    ![image.png](/images/image%201.png)
    
    3.输入项目名称
    
    ![image.png](/images/image%202.png)
    

---

- 系统基础软/硬件配置(不包含定时器TIM配置)
    
    1.内外部晶振选择
    
    ![image.png](/images/image%203.png)
    
    2.仿真器配置
    
    ![image.png](/images/image%204.png)
    
    3.LED引脚配置(推挽输出模式，初始化为低电平)
    
    > 根据原理图可知引脚对应关系
    > 
    
    | 引脚 | 元件 | User        Label |
    | --- | --- | --- |
    | PC0 | LED0 | GPO_LED_0 |
    | PC1 | LED1 | GPO_LED_1 |
    | PC2 | LED2 | GPO_LED_2 |
    | PC3 | LED3 | GPO_LED_3 |
    | PC4 | LED4 | GPO_LED_4 |
    | PC5 | LED5 | GPO_LED_5 |
    | PC6 | LED6 | GPO_LED_6 |
    | PC7 | LED7 | GPO_LED_7 |
    
    ![image.png](/images/image%205.png)
    
    <aside>
    💡
    
    如上图同理配置其余7个LED引脚
    
    </aside>
    
    4.配置按键引脚
    
    | 引脚 | 元件 | 模式 | User        Label |
    | --- | --- | --- | --- |
    | PC8 | KEY0 | 输入模式+上拉电阻 | GPI_KEY_LEFT |
    | PC9 | KEY1 | 输入模式+上拉电阻 | GPI_KEY_DOWN |
    | PD2 | KEY2 | 输入模式+上拉电阻 | GPI_KEY_RIGHT |
    | PA0 | WK_UP | 输入模式+下拉电阻 | GPI_KEY_UP |
    
    ![image.png](/images/image%206.png)
    
    <aside>
    💡
    
    如上图同理配置其他3个按键
    
    </aside>
    
    5.配置数码管引脚(推挽输出模式)
    
    | 引脚 | 元件 | User        Lable |
    | --- | --- | --- |
    | PB3 | HC595_DATA | GPO_HC595_PB3 |
    | PB4 | HC595_LCLK | GPO_HC595_PB4 |
    | PB5 | HC595_SCLK | GPO_HC595_PB5 |
    | PC10 | HC138_A0 | GPO_HC138_A0 |
    | PC11 | HC138_A1 | GPO_HC138_A1 |
    | PC12 | HC138_A2 | GPO_HC138_A2 |
    
    ![image.png](/images/image%207.png)
    
    <aside>
    💡
    
    如上图配置其余五个引脚
    
    </aside>
    
    6.配置蜂鸣器引脚
    
    ![image.png](/images/image%208.png)
    
    <aside>
    💡
    
    一般一块板子就一个蜂鸣器，这里是有源的
    
    </aside>
    
    7.配置UART为DMA模式
    
    ![image.png](/images/image%209.png)
    
    接着使能UART中断
    
    ![image.png](/images/image%2010.png)
    
    再修改通讯波特率
    
    ![image.png](/images/image%2011.png)
    

---

- 时钟树配置
    
    1.
    
    ![image.png](/images/image%2012.png)
    
    输入频率为8MHZ与波特率为115200相关，若不配置为8M，后面传输会出现波特率不匹配的问题
    

---

- 生成初始化代码
    
    ![image.png](/images/image%2013.png)
    
    按齿轮或者Ctrl+S即可自动生成初始代码
    

---

- 新建头文件夹、源文件夹路径
    
    **右键工程→点击propertrie**s
    
    ![image.png](/images/image%2014.png)
    
    ![image.png](/images/image%2015.png)
    
    这样以后自己编写的.h、.c文件才能被程序找到(新建驱动后可如下图放置)
    
    ![image.png](/images/image%2016.png)
    

---

- daplink下载配置
    
    ![image.png](/images/image%2017.png)
    
    ![image.png](/images/image%2018.png)
    
    **修改cfg配置文件如下：**
    

---

## 驱动编写

### LED驱动

- led.h
    
    ```c
    /*
    * led.h
    *
    *  Created on: Oct 29, 2024
    *      Author: kurumi
    */
    
    **#ifndef** LED_H_
    **#define** LED_H_
    **#include** "main.h"
    **#define** LED_0   0x01
    **#define** LED_1   0x02
    **#define** LED_2   0x04
    **#define** LED_3   0x08
    **#define** LED_4   0x10
    **#define** LED_5   0x20
    **#define** LED_6   0x40
    **#define** LED_7   0x80
    **#define** LED_ALL 0xFF
    **void** **ledOn**(uint8_t val);
    **void** **ledOff**(uint8_t val);
    **void** **ledToggle**(uint8_t val);
    **#endif** /* LED_H_ */
    ```
    
- 定义结构体访问引脚
    
    ```c
    struct GPIO_LED{
    	GPIO_TypeDef* port;
    	uint16_t pin;
    };
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
    ```
    
- 开启led
    
    ```c
    void ledOn(uint8_t val){
    	for(uint8_t i = 0; i < NUM_LEDS; i++){
    		if(val & (0x01 << i))
    			{HAL_GPIO_WritePin(led[i].port, led[i].pin, RESET);}
    	}
    }
    ```
    
- 关闭led
    
    ```c
    void ledOff(uint8_t val){
    	for(uint8_t i = 0; i < NUM_LEDS; i++){
    			if(val & (0x01 << i))
    				{HAL_GPIO_WritePin(led[i].port, led[i].pin, SET);}
    		}
    }
    ```
    
- led状态切换
    
    ```c
    void ledToggle(uint8_t val){
    	for(uint8_t i = 0; i < NUM_LEDS; i++){
    				if(val & (0x01 << i))
    					{HAL_GPIO_TogglePin(led[i].port, led[i].pin);}
    			}
    }
    ```
    
- 二进制显示得分,由于led有8个，固可以用8位二进制数是否为0、1来判断哪些led被点亮，只需提前记录一个score值，然后吃到食物后score++,ledOn(score)，ledOff(score)即当前分数对应的二进制值
    
    

### 蜂鸣器驱动

- beep.h
    
    ```c
    /*
     * beep.h
     *
     *  Created on: Nov 6, 2024
     *      Author: kurumi
     */
    #include "main.h"
    #ifndef BEEP_H_
    #define BEEP_H_
    void beepInit();
    #endif
    ```
    
- 配置蜂鸣器引脚为定时器的PWM模式(复用推挽输出模式)
    
    ![image.png](/images/image%2019.png)
    
- 设置为向下计数模式，重装载值设置为100，pwm的频率就是10000HZ，当计数值小于Pulse值为有效电平，大于等于Pulse值为无效电平
    
    ![image.png](/images/image%2020.png)
    
    如下面配置代码即可实现蜂鸣器的呼吸鸣叫功能,通过调节占空比实现
    
    ```c
    		 for(uint8_t i =0; i<100; i++){
    			 HAL_Delay(10);
    			 __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, i);
    		 }
    		 for(uint8_t i =99; i>=0; i--){
    		 			 HAL_Delay(10);
    		 			 __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, i);
    		 }
    ```
    
- 吃到食物后先开启蜂鸣器，再叫一声后关闭蜂鸣器，还可通过改变比较值来做出呼吸鸣叫的效果
    
    ```c
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
    ```
    

### 按键驱动

- key.h
    
    ```c
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
    ```
    
- 按键初始化(使用中断来周期性响应按键状态)
    
    > 中断配置(PA1引脚作为TIM2)
    > 
    
    ![image.png](/images/image%2021.png)
    
    ![image.png](/images/image%2022.png)
    
    通过预分频值与重装载值来计算定时周期((预分频*重装载)/系统主频(100M)=50ms)
    
    ```c
    void keyInit(void){
    	HAL_TIM_Base_Start_IT(&htim2);
    }
    ```
    
- 按键响应函数
    
    ```c
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
    ```
    
- 读取按键值函数
    
    ```c
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
    ```
    
    ![image.png](/images/image%2023.png)
    
    <aside>
    ⚙
    
    将按键响应函数放入定时器中断函数
    
    </aside>
    

### 数码管驱动

![image.png](/images/image%2024.png)

           由原理图可知，数码管是由74HC138(PC10\PC11\PC12)实现的段选功能,由74HC595实现的位选功能(PB3\PB4\PB5)。首先在74HC595的Data处输入一位数字，再给SFTCLK一个上升沿脉冲，就会触发移位寄存功能，如此重复8次，即输入8位数字，最后给LCHCLK一个上升沿脉冲即会把移位寄存器锁存到输出寄存器。

- digitron.h
    
    ```c
    #ifndef __DIGITRON_H
    #define __DIGITRON_H
    #include "main.h"
    #ifdef DIGITRON_GLOBAL
    	#define DIGITRON_EXTERN
    #else
    	#define DIGITRON_EXTERN extern
    #endif
    
    void latchClk(void);
    void chooseDigitron(uint8_t);
    void digitronPutBuf(uint8_t, uint8_t );
    void putData(uint8_t);
    void displayToken(uint8_t, uint8_t);
    
    #endif
    ```
    
- 单个数码管显示函数
    
    
    ![image.png](/images/image%2025.png)
    
    若输入0x87,则说明数码管的A、B、C、DP段被选择
    
    ```c
    void putData(uint8_t segments) {
        for(uint8_t i = 0; i < 8; i++) {
            HAL_GPIO_WritePin(HC595[0].port, HC595[0].pin, (segments >> (7-i)) & 0x01);
            sftClk();
        }
    }
    ```
    
- 移位和锁存上升沿脉冲函数
    
    ```c
    void sftClk(void){
    	HAL_GPIO_WritePin(HC595[2].port, HC595[2].pin, RESET);
    	HAL_GPIO_WritePin(HC595[2].port, HC595[2].pin, SET);
    }
    
    //给PB4引脚一个上升沿
    void latchClk(void){
    	HAL_GPIO_WritePin(HC595[1].port, HC595[1].pin, RESET);
    	HAL_GPIO_WritePin(HC595[1].port, HC595[1].pin, SET);
    }
    
    ```
    
- 数码管段选函数
    
    ```c
    void chooseDigitron(uint8_t position){
    	for(uint16_t i = 0; i < 3; i++){
    		HAL_GPIO_WritePin(HC183[i].port, HC183[i].pin, (position >> i) & 0x01);
    	}
    }
    ```
    
- 动态显示函数
    
    由于数码管硬件设计的局限性，导致其在某个时刻只能有一个管子是亮的，但是又因为人眼的刷新频率，只要在极短的时间内让每个管子依次亮起(每个管子可以亮不同的符号，也可以不亮),就会形成一种8个数码管同时显示不同符号的错觉，除非你加大了依次亮起的时间间隔，否则你将观察不到数码管的闪烁现象。
    
    ```c
    void displayToken(uint8_t n, uint8_t section) {
    	HAL_Delay(0.5);
    	putData(section);
    	latchClk();
        chooseDigitron(n);
    }
    ```
    

### 贪吃蛇驱动

贪吃蛇是非常经典的一款游戏，本次我们模拟在数码管中实现贪吃蛇游戏，用“↑↓←→”按键控制移动蛇的方向，吃掉食物之后，蛇身体会变长；数码管左右不设边界，但是上下会有墙壁，蛇头碰撞检测等等。

---

首先，我们得清楚蛇的运动逻辑和游戏中会出现的所有情况

1. 蛇的身体是一段一段的，很容易联想到链表这个数据结构，我们只需要先初始化一个蛇头，然后在长度增加时创建一个新的节点并将头节点指向这个节点即可
2. 蛇的移动：相较于普通的屏幕上的显示，在数码管中不能采用坐标的形式记录蛇的位置，固而采用了通过制定一张查找表的方式来更新下一步蛇身的状态，查找表是根据当前蛇头在数码管的第n段、当前方向和上一步的方向来更新下一步蛇的位置。同时，蛇头之外的其他段下一步的位置只要继承他们前一段上一步的位置即可
3. 食物的随机生成：游戏初始化时会在随机位置显示食物，通过HAL_GetTick函数即可获取>0的随机值，同时还要注意食物生成的位置不能与蛇重合
4. 蛇的长度增加: 当蛇头与食物重合时，立即在蛇头后面加一段身体
5. 蛇的碰撞检测:数码管的上下设有墙壁，左右可穿墙而过，只要判断蛇头在在某个位置停留时间>蛇位置刷新的时间即可知道蛇是否撞墙而死；除了墙壁，蛇头撞向自身时也会触发死亡，由于没有坐标，因此人为对数码管建立坐标系，每个数码管的7段(DP位用不到)可分为6个点来记录，这样，只要蛇头的坐标和任意一段蛇身的坐标重合即可判断蛇撞向了自己。但是蛇头坐标与墙壁坐标重合不能判断蛇是否死亡，因为蛇此时还可以由玩家控制转向贴墙而行。
- snake.h
    
    ```c
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
    #endif /* SNAKE_H_ */
    
    ```
    
- 位置更新表
    
    ```c
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
    ```
    
- 创建蛇的每个块
    
    ```c
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
    ```
    
- 添加蛇的每个块
    
    ```c
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
    ```
    
- 更新按键按下后蛇头的朝向
    
    ```c
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
    ```
    
- 朝向更新完毕后更新蛇的每个块的位置
    
    ```c
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
    ```
    
- 随机生成食物
    
    ```c
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
    ```
    
- 判断食物是否被吃掉及是否增加长度
    
    ```c
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
    ```
    

### 界面显示驱动

- 此项目中设计的UI界面共三个，包含开始界面、运行界面与结算界面
    
    只要注意在运行界面时开启更新贪吃蛇的中断，在其他界面时关闭更新贪吃蛇的中断即可。
    

### UART-DMA驱动

- 使用的是DMA+空闲中断来接受不定长数据，所谓空闲状态就是指数据接受完毕后，若在一个接收一字节数据的时间内没有新的数据被接收到，则产生一个IDLE中断；但需要注意的是，若接受的数据长度大于缓存接收数组长度的一半同样也会触发该中断，所以我禁用了这个过半中断
    
    ```c
    	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxData, sizeof(rxData));//等待接收
     __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);		//关闭传输过半触发的中断
    ```
    
    数据传送完成中断回调函数与空闲中断回调函数在HAL库中原本是弱定义的，此时需要我们自己去定义这两个函数来实现我们想要的功能。我在空闲中断回调函数中添加了发送接收到的数据、等待下一次接收与关闭过半中断的功能，这样就能重复接收我们发的数据了。
    
    ```c
    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
    {
        if (huart->Instance == USART1)
        {
        	HAL_UART_Transmit_DMA(&huart1, rxData, size);
        	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxData, sizeof(rxData));
        	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
        }
    }
    ```
    
- 同时，又在传输完成中断回调函数中添加了判断数据帧头(我设为了’[’)和帧尾(我设为了’]’)是否符合要求的功能，若发送格式不对，则在XCOM界面打印ERROR，若正确则解析数据
    
    ```c
    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
    {
        if (huart->Instance == USART1)
        {
        	dataLength = size;
        	if(rxData[0] =='[' && rxData[dataLength-1] == ']'){
        		switch(rxData[1]){
        		case '0':
        			mode = START;
        			break;
        		case '1':
        			mode = RUNNING;
        		case '2':
        			mode = PAUSE;
        		case '3':
        			mode = END;
        		default:
        			break;
        		}
        	}
        	else{
        		printf("Transmit ERROR!\r\n");
        		 }
        	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxData, sizeof(rxData));
        	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
        }
    }
    
    ```
    

# 3.CHECK

### 关于数码管显示

起初打算用一个长度为8的数组来记录每个数码管应该亮起的位，但后来觉得过于复杂了，遂决定用链表的形式来表示蛇，这样每次只要显示一个数码管的一位就行。

### 关于按键响应

起初做的时候发现按键的状态虽然能读取到，但都是一瞬间的，往往在蛇的方向还没有更新前，读取到的按键值就恢复成了NULL，固加入了一个蛇的位置更新标志位，在位置更新前，按键被按下后将会保留该按键值，位置更新后标志位清零

### 关于头文件依赖

编译的时候是按照main.h中include头文件的顺序来依次编译的，链接是按照字母从小到大排的顺序链接的，因此，若某个文件用到了另一个文件的变量，那另一个文件应该在这个文件之前被include。

### 关于重复依赖

snake.c用到了zoord.h中声明的枚举，而zoord.c的一个函数中又用到了snake.c中的变量，此时会出现重复依赖问题，因为头文件重复包含时，不会因为定义了

```c
#ifndef SNAKE_H_
#define SNAKE_H_
#endif
```

就取消重复包含，在预编译的过程中snake和zoord这两个文件会一直重复包含，导致访问不到变量，固新建了一个shared.h文件来让snake和zoord来共用shared.h中的声明。

### 关于随机食物生成

由于随机数是基于时间生成的伪随机，固无法解决每次游戏开始时食物的出生点都一样，除非采用读取噪声影响下的悬空引脚的电平来实现真随机。

### 关于PWM

用PWM来控制蜂鸣器时，若占空比变化过快，会导致基本听不清蜂鸣器的声音，添加些许延时后解决了此问题。

### 关于UART

使用空闲中断回调函数来重复接收数据并解析数据，使用串口软件时发现看上去发送的是符合自己格式的数据，但用printf把接收的数组一个个打印出来后发现由上位机发送的数据后面还跟着\r\n导致多了两位。

# 4.ACT

1. 查看数据手册和原理图了解各种元件的原理与配置方式
2. 编写各种元件的驱动，来模块化编程，方便移植
3. 规范化代码命名规则和注释要求
4. 加上版本管理控制，既能回到记录点，又能知道自己在哪些地方做出了改变
5. 总结软件Bug经验和技术难点突破
6. 用串口调试有时比自己想逻辑错误容易许多，能节省不必要的时间开销
7. 后续还能增加一些新的特性，比如加速后蛇的长度会减小；改进用户体验；提高系统灵敏度；
8. 与小组成员分享经验并讨论改进建议

# 实习心得

两个星期前，对嵌入式实习怀着满腔热忱，觉得两个星期完全足够做完一个入门项目了。于是便开始着手计划任务，本来打算的是做一个类似于谷歌地离线恐龙跑酷小游戏，但是跟同学讨论后发现有人跟我撞创意了，于是我便决定改做贪吃蛇游戏了，前两天也没什么事，基本就是写一些简单的按键、led、数码管驱动，有空还能看别人遇到的bug并帮助一起解决了，也算是经验的累积了。这两天最耗时地应该还是按键驱动的调试吧，有些按键按了没反应，通过用led地亮灭来判断按键响应的问题出现在哪个阶段来一步步锁定问题，我记得当时是觉得按键状态跳到KEY_UP_LONG后会立即跳到KEY_NULL状态而出现的问题，加了个标志位后解决了。第三天又开始纠结该如何记录贪吃蛇的状态，刚开始没想到用链表，导致有些功能得不到实现，然后在第4天的早上灵光乍现，由动态分配一下子联想到链表，当时就感觉好像所有问题都迎刃而解了，直到晚上写完整体框架后运行程序才发现数码管并没有按照设想的方式运行，只能一遍遍从头看自己写的代码，但是看了很多遍又没发现逻辑问题，心态是有一点点崩的，过了半天后才突然意识到数码管的位索引是按照8位二进制数来的，而不是0-7，取了个对数后就好了。此时应该是星期五的中午了，看到贪吃蛇在数码管上正常显示后顿感一身轻松，于是又去到处看别人做的进度了，当时差不多有好多人都已经基本做完了。第二周实习开始，我开始投入到研究如何判断蛇是否撞向自己，一开始想的是蛇头与身体的某段重合就算撞到了，但考虑的情况并不是很完整，因为蛇头也不能从身体中穿过去，这就让人很迷茫了，因为数码管也没有现成的坐标系来用，想了一天后决定还是自己为数码管建立坐标系后通过判断坐标是否重合来写碰撞检测逻辑。写完碰撞检测逻辑后此时还没有写食物生成和蛇的长度增加逻辑，所以验证不了碰撞检测逻辑，于是又花了一天的时间来写随机生成食物和为蛇添加身体的代码(食物不能生成在蛇的体内)。到了周四开始验证碰撞逻辑，发现基本没什么问题。接着又花了一下午的时间把UI界面、难度选择、计分、音乐、暂停等功能写完了。本来是20：00前就能下班的，但是因为周五要验收，又留下来陪其他人写代码和文档，由于错估了UART的难度，导致研究老师的代码到了通宵才基本解决完所有问题，最后留了最后一天来写自己的串口通讯自定义功能，发现用串口打印变量的值还挺方便的，能比平常解决问题节省不少时间。总的来说，这次实习还是挺充实的，能帮助新手较快地入门嵌入式(纸上谈兵不如躬身实践)。但是写文档的时候基本上已经是项目快做完了，期间调试时遇到的许多问题都没有及时记录下来，导致后面写文档的时候都忘得差不多了。还有一件事，本身大家对PDCA的规范格式不是很清楚，下次老师可以提前讲清楚PDCA应该怎么写，而不是到最后一天再去调整大家的文档规范内容。

> 路漫漫其修远兮，吾将上下而求索。
> 

***本作品已在github开源***

**Source**: [https://github.com/Wio2let/stm32f411rct6](https://github.com/Wio2let/stm32f411rct6)