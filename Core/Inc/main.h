/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "digitron.h"
#include "beep.h"
#include "key.h"
#include "led.h"
#include "snake.h"
#include "mode.h"
#include "zoord.h"




/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPO_LED_0_Pin GPIO_PIN_0
#define GPO_LED_0_GPIO_Port GPIOC
#define GPO_LED_1_Pin GPIO_PIN_1
#define GPO_LED_1_GPIO_Port GPIOC
#define GPO_LED_2_Pin GPIO_PIN_2
#define GPO_LED_2_GPIO_Port GPIOC
#define GPO_LED_3_Pin GPIO_PIN_3
#define GPO_LED_3_GPIO_Port GPIOC
#define GPI_KEY_UP_Pin GPIO_PIN_0
#define GPI_KEY_UP_GPIO_Port GPIOA
#define GPO_LED_4_Pin GPIO_PIN_4
#define GPO_LED_4_GPIO_Port GPIOC
#define GPO_LED_5_Pin GPIO_PIN_5
#define GPO_LED_5_GPIO_Port GPIOC
#define GPO_LED_6_Pin GPIO_PIN_6
#define GPO_LED_6_GPIO_Port GPIOC
#define GPO_LED_7_Pin GPIO_PIN_7
#define GPO_LED_7_GPIO_Port GPIOC
#define GPI_KEY_LEFT_Pin GPIO_PIN_8
#define GPI_KEY_LEFT_GPIO_Port GPIOC
#define GPI_KEY_DOWN_Pin GPIO_PIN_9
#define GPI_KEY_DOWN_GPIO_Port GPIOC
#define GPO_HC183_PA0_Pin GPIO_PIN_10
#define GPO_HC183_PA0_GPIO_Port GPIOC
#define GPO_HC183_PA1_Pin GPIO_PIN_11
#define GPO_HC183_PA1_GPIO_Port GPIOC
#define GPO_HC183_PA2_Pin GPIO_PIN_12
#define GPO_HC183_PA2_GPIO_Port GPIOC
#define GPI_KEY_RIGHT_Pin GPIO_PIN_2
#define GPI_KEY_RIGHT_GPIO_Port GPIOD
#define GPO_HC595_PB3_Pin GPIO_PIN_3
#define GPO_HC595_PB3_GPIO_Port GPIOB
#define GPO_HC595_PB4_Pin GPIO_PIN_4
#define GPO_HC595_PB4_GPIO_Port GPIOB
#define GPO_HC595_PB5_Pin GPIO_PIN_5
#define GPO_HC595_PB5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
