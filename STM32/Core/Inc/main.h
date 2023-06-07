/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
enum motor_e {MOTOR_X = 0, MOTOR_Y = 1, MOTOR_ZOOM = 2};

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
TIM_HandleTypeDef* get_htim3();
TIM_HandleTypeDef* get_htim4();
TIM_HandleTypeDef* get_htim8();
I2C_HandleTypeDef* get_hi2cl();
UART_HandleTypeDef* get_huart2();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MODE_ZOOM_1_Pin GPIO_PIN_14
#define MODE_ZOOM_1_GPIO_Port GPIOC
#define MODE_ZOOM_2_Pin GPIO_PIN_15
#define MODE_ZOOM_2_GPIO_Port GPIOC
#define MODE_ZOOM_3_Pin GPIO_PIN_2
#define MODE_ZOOM_3_GPIO_Port GPIOC
#define DIR_MOTOR_ZOOM_Pin GPIO_PIN_3
#define DIR_MOTOR_ZOOM_GPIO_Port GPIOC
#define MODE_Y_2_Pin GPIO_PIN_5
#define MODE_Y_2_GPIO_Port GPIOA
#define MODE_Y_1_Pin GPIO_PIN_6
#define MODE_Y_1_GPIO_Port GPIOA
#define DIR_MOTOR_X_Pin GPIO_PIN_4
#define DIR_MOTOR_X_GPIO_Port GPIOC
#define MODE_X_3_Pin GPIO_PIN_12
#define MODE_X_3_GPIO_Port GPIOB
#define DIR_MOTOR_Y_Pin GPIO_PIN_10
#define DIR_MOTOR_Y_GPIO_Port GPIOA
#define MODE_X_2_Pin GPIO_PIN_11
#define MODE_X_2_GPIO_Port GPIOA
#define MODE_X_1_Pin GPIO_PIN_12
#define MODE_X_1_GPIO_Port GPIOA
#define MODE_Y_3_Pin GPIO_PIN_9
#define MODE_Y_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
