/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
typedef enum
{
    ADC2_CHANNEL_CURRENT = 0,
    ADC2_CHANNEL_CHARGEVOLTAGE,
    ADC2_CHANNEL_BATTERYVOLTAGE,
    ADC2_CHANNEL_CHARGERINPUTVOLTAGE,
    ADC2_CHANNEL_NTC,
    ADC2_CHANNEL_PERIMETER,
    ADC2_CHANNEL_MAX,
} ADC_channelSelection_e;

union FtoU{
  float  f;
  uint16_t u[2];
};

extern float battery_voltage;
extern float charge_voltage;
extern float current;
extern float current_without_offset;
extern float blade_temperature;
extern float chargerInputVoltage;

//extern union FtoU ampere_acc;  //used for SOC that needs refactoring
extern union FtoU charge_current_offset;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void adc_SetChannel(ADC_channelSelection_e channel);
void ADC_Update(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

