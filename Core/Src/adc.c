/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "rtc.h"
#include "tim.h"
#include <math.h>

const float f_RTO = 10000;
const float beta = 3380;

ADC_channelSelection_e adc_eChannelSelection = ADC2_CHANNEL_CURRENT;

volatile uint16_t adc_u16BatteryVoltage       = 0;
volatile uint16_t adc_u16Current              = 0;
volatile uint16_t adc_u16ChargerVoltage       = 0;
volatile uint16_t adc_u16ChargerInputVoltage  = 0;
volatile uint16_t adc_u16Input_NTC            = 0;


float battery_voltage;
float charge_voltage;
float current;
float current_without_offset;
float ntc_voltage;
float blade_temperature;
float chargerInputVoltage;

union FtoU ampere_acc;
union FtoU charge_current_offset;


/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */
  
    
  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */


   //if(HAL_ADCEx_Calibration_Start(&hadc1 ) != HAL_OK){
   //    Error_Handler();
   //}
  if(HAL_ADC_Start_IT(&hadc1) != HAL_OK){
        Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
                Error_Handler();
  }
                

    ampere_acc.u[0] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
    ampere_acc.u[1] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);

    charge_current_offset.u[0] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
    charge_current_offset.u[1] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC2     ------> ADC1_IN12
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PA6     ------> ADC1_IN6
    PA7     ------> ADC1_IN7
    */
    GPIO_InitStruct.Pin = Blade_NTC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(Blade_NTC_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Charge_Current_Pin|Charge_Voltage_Pin|Battery_Voltage_Pin|Perimeter_Pin
                          |Charger_Input_Voltage_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC2     ------> ADC1_IN12
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PA6     ------> ADC1_IN6
    PA7     ------> ADC1_IN7
    */
    HAL_GPIO_DeInit(Blade_NTC_GPIO_Port, Blade_NTC_Pin);

    HAL_GPIO_DeInit(GPIOA, Charge_Current_Pin|Charge_Voltage_Pin|Battery_Voltage_Pin|Perimeter_Pin
                          |Charger_Input_Voltage_Pin);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

    if (hadc == &hadc1)
    {
        uint16_t l_u16Rawdata = hadc1.Instance->DR;

        switch (adc_eChannelSelection)
        {
        case ADC2_CHANNEL_CURRENT:
            adc_u16Current = l_u16Rawdata;
            break;

        case ADC2_CHANNEL_CHARGEVOLTAGE:
            adc_u16ChargerVoltage = l_u16Rawdata;
            break;

        case ADC2_CHANNEL_BATTERYVOLTAGE:
            adc_u16BatteryVoltage = l_u16Rawdata;
            break;

        case ADC2_CHANNEL_CHARGERINPUTVOLTAGE:
            adc_u16ChargerInputVoltage = l_u16Rawdata;
            break;

        case ADC2_CHANNEL_NTC:
            adc_u16Input_NTC = l_u16Rawdata;
            break;

        case ADC2_CHANNEL_PERIMETER:
            //PERIMETER_vITHandle();
            break;

        case ADC2_CHANNEL_MAX:
        default:
            /* should not get here */
            break;
        }

        adc_eChannelSelection++;
        if (adc_eChannelSelection == ADC2_CHANNEL_MAX)
            adc_eChannelSelection = ADC2_CHANNEL_CURRENT;
        adc_SetChannel(adc_eChannelSelection);

        HAL_ADC_Start_IT(&hadc1);
    }
}


void adc_SetChannel(ADC_channelSelection_e channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    switch (channel)
    {
    case ADC2_CHANNEL_CURRENT:
        sConfig.Channel = ADC_CHANNEL_1; // PA1 Charge Current
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    case ADC2_CHANNEL_CHARGEVOLTAGE:
        sConfig.Channel = ADC_CHANNEL_2; // PA2 Charge Voltage
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    case ADC2_CHANNEL_BATTERYVOLTAGE:
        sConfig.Channel = ADC_CHANNEL_3; // PA3 Battery
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    case ADC2_CHANNEL_CHARGERINPUTVOLTAGE:
        sConfig.Channel = ADC_CHANNEL_7; // PA7 Charger Input voltage
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    case ADC2_CHANNEL_NTC:
        sConfig.Channel = ADC_CHANNEL_12; // PC2
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    case ADC2_CHANNEL_MAX:
    default:
        /* should not get here */
        sConfig.Channel = ADC_CHANNEL_3; // PA3 Battery
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    }
}

void ADC_Update(void)
{
    float l_fTmp;

    /* battery volatge calculation */
    l_fTmp = ((float)adc_u16BatteryVoltage / 4095.0f) * 3.3f * 10.09 + 0.6f;
    battery_voltage = 0.4 * l_fTmp + 0.6 * battery_voltage;

     /*charger voltage calculation */
    l_fTmp = ((float)adc_u16ChargerVoltage / 4095.0f) * 3.3f * 16;
    charge_voltage = 0.8 * l_fTmp + 0.2 * charge_voltage;

    /*charge current calculation */
    l_fTmp = (((float)adc_u16Current / 4095.0f) * 3.3f - 2.5f) * 100 / 12.0;
    current_without_offset =   0.8 * l_fTmp + 0.2 * current_without_offset;          

    /*remove offset*/
    current = current_without_offset - charge_current_offset.f;

    /*blade motor temperature calculation */
    l_fTmp = (adc_u16Input_NTC/4095.0f)*3.3f;
    ntc_voltage = 0.5*l_fTmp + 0.5*ntc_voltage;

    /*calculation for NTC temperature*/
    l_fTmp = ntc_voltage * 10000;               //Resistance of RT
    l_fTmp = log(l_fTmp / f_RTO);
    l_fTmp = (1 / ((l_fTmp / beta) + (1 / (273.15+25)))); //Temperature from thermistor
    blade_temperature = l_fTmp - 273.15;                 //Conversion to Celsius  

    /* Input voltage from the external supply*/
    l_fTmp = (adc_u16ChargerInputVoltage / 4095.0f) * 3.3f * (32 / 2);
    chargerInputVoltage = 0.5 * l_fTmp + 0.5 * chargerInputVoltage;

}

/* USER CODE END 1 */
