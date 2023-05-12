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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdint.h>
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
void logSerial(uint8_t *message);
void parseSerialCommand(uint8_t *command);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Blade_NTC_Pin GPIO_PIN_2
#define Blade_NTC_GPIO_Port GPIOC
#define Charge_Current_Pin GPIO_PIN_1
#define Charge_Current_GPIO_Port GPIOA
#define Charge_Voltage_Pin GPIO_PIN_2
#define Charge_Voltage_GPIO_Port GPIOA
#define Battery_Voltage_Pin GPIO_PIN_3
#define Battery_Voltage_GPIO_Port GPIOA
#define Perimeter_Pin GPIO_PIN_6
#define Perimeter_GPIO_Port GPIOA
#define Charger_Input_Voltage_Pin GPIO_PIN_7
#define Charger_Input_Voltage_GPIO_Port GPIOA
#define High_Voltage_Enable_Pin GPIO_PIN_5
#define High_Voltage_Enable_GPIO_Port GPIOC
#define Led_D3_Pin GPIO_PIN_2
#define Led_D3_GPIO_Port GPIOB
#define Driver_Motor_Enable_Pin GPIO_PIN_15
#define Driver_Motor_Enable_GPIO_Port GPIOE
#define Driver_Motor_EnableD8_Pin GPIO_PIN_8
#define Driver_Motor_EnableD8_GPIO_Port GPIOD
#define Wheel_lift_blue_Pin GPIO_PIN_0
#define Wheel_lift_blue_GPIO_Port GPIOD
#define Wheel_lift_red_Pin GPIO_PIN_1
#define Wheel_lift_red_GPIO_Port GPIOD
#define Driver_Motor_EnableD7_Pin GPIO_PIN_7
#define Driver_Motor_EnableD7_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
