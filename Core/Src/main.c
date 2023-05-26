/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "wwdg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "drivemotor.h" 
#include "blademotor.h" 
#include "emergency.h" 
#include "charger.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t usb_serial_command[64];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef enum {
    DISARMED=0,
    ARMED
}STATE_e;


STATE_e status = DISARMED;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_WWDG_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  logSerial("System Boot Completed!\n");
  logSerial("Services Initializations started...\n");
  memset (usb_serial_command, '\0', 64);  // clear the buffer
  HAL_GPIO_WritePin(Led_D3_GPIO_Port, Led_D3_Pin,1);
  
  /* init Status update variables*/
  int cycle = 0;

  /* Enable high voltage*/
  HAL_GPIO_WritePin(High_Voltage_Enable_GPIO_Port, High_Voltage_Enable_Pin, 1);
  logSerial("High Voltage Circuit: On\n");


  /* Initialize Driver Motors ESC */
  DRIVEMOTOR_Init();
  logSerial("Driver Motors: Ready\n");
    /* Initialize Blade Motors ESC */
  BLADEMOTOR_Init();
  logSerial("Blade Motor: Ready\n");

  /* Initilize Charge Controler*/
  TIM1->CCR1 = 0;  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  logSerial("Charge Controler PWM Timers initialized!\n");

  /* Initializing Charger*/
  CHARGER_Init();

  /* Initializing Buzzer*/
  TIM3->CCR4 = 0;  
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  BUZZER_SET(1);
  HAL_Delay (500);
  BUZZER_SET(0);
  logSerial("Buzzer initialized!\n");

  EMERGENCY_Init();
  logSerial("Services Initializations completed!\n");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    

    if (usb_serial_command[0] != '\0')
    {
       parseSerialCommand(usb_serial_command);
       memset (usb_serial_command, '\0', 64);  // clear buffer
    }


    ADC_Update();
    EMERGENCY_Update();
    CHARGER_Update();

    if (EMERGENCY_State())
		{
      status=DISARMED;
		}

    //Override motors command to hidle
    if(status == DISARMED){
      DRIVEMOTOR_SetSpeed(0, 0);
			BLADEMOTOR_Set(0);
    }
    DRIVEMOTOR_Run();
    BLADEMOTOR_Run();

    cycle++;
    if(cycle%10==0){  //100ms
      HAL_GPIO_TogglePin (Led_D3_GPIO_Port, Led_D3_Pin);
    }
    if(cycle%100==0){ //1s
        STATE_Send();
    }
    if(cycle>1000){
      cycle=1;
    }
    HAL_WWDG_Refresh(&hwwdg);
    HAL_Delay (10);   /* Insert delay 10 ms */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/*
 * Log USB Serial
 */
void logSerial(uint8_t *message)
{
 CDC_Transmit_FS(message, strlen(message));
}


void parseSerialBuffer(uint8_t *buffer) {


    // Returns first token
    char* command = strtok(buffer, "\n");
 
    while (command != NULL) {
        parseSerialCommand(command);
        command = strtok(buffer, "\n");
    }

}



void parseSerialCommand(uint8_t *command) {
    static char buffer[50];
    if (strncmp(command, "speed:", 6) == 0) {
        float left, right;
        if(status!=ARMED){
          logSerial("Warning this command requires ARMED state\n");
        }else{
           //TODO: this is not working
          int numMatches = sscanf(command+6, "%f %f", &left, &right);
          if (numMatches == 2) {
              DRIVEMOTOR_SetSpeed(left, right);
              sprintf(buffer, "Set speed left to:%.2f and right to:%.2f.\n",left,right);
              logSerial((uint8_t *)buffer);
          } else {
              logSerial("Invalid speed command format: '"); logSerial(command); logSerial("'\n");
          }
        }
    } else if (strncmp(command, "cut", 3) == 0) {
        uint8_t charger_on_off;
        if(status!=ARMED){
          logSerial("Warning this command requires ARMED state\n");
        }else{
            int numMatches = sscanf(command+8, "%d", &charger_on_off);
            if (numMatches == 1) {
                BLADEMOTOR_Set(charger_on_off);
                sprintf(buffer, "Set charger:%d\n",charger_on_off);
                logSerial((uint8_t *)buffer);
            } else {
                logSerial("Invalid speed command format: '"); logSerial(command); logSerial("'\n");
            }
        }
    } else if (strncmp(command, "arm", 3) == 0) {
        status=ARMED;
    } else if (strncmp(command, "charger:", 8) == 0) {
        uint8_t charger_on_off;
        int numMatches = sscanf(command+8, "%d", &charger_on_off);
        if (numMatches == 1) {
            CHARGER_Set(charger_on_off);
            sprintf(buffer, "Set charger:%d\n",charger_on_off);
            logSerial((uint8_t *)buffer);
        } else {
            logSerial("Invalid speed command format: '"); logSerial(command); logSerial("'\n");
        }
    } else if (strncmp(command, "halt", 4) == 0) {
        status=DISARMED;
        logSerial("Ack! Halting!");
    } else if (strncmp(command, "ack", 3) == 0) {
       EMERGENCY_SerialAck();
    } else {
        logSerial("Unknown command:'"); logSerial(command); logSerial("'\n");
    }
}

int RAIN_Sense(void)
{
  return (!HAL_GPIO_ReadPin(Rain_Sensor_GPIO_Port, Rain_Sensor_Pin)); // pullup, active low
}

int BUTTON_Home(void)
{
  return (!HAL_GPIO_ReadPin(Home_Button_GPIO_Port, Home_Button_Pin)); // pullup, active low
}

int BUTTON_Play(void)
{
  return (!HAL_GPIO_ReadPin(Play_Button_GPIO_Port, Play_Button_Pin)); // pullup, active low
}


void BUZZER_SET(uint8_t on_off){

      if (on_off)
      {
        TIM3->CCR4 = 10; // chirp on
      }else{
        TIM3->CCR4 = 0; // chirp off
      }
    }

void STATE_Send() {

  static char status_buffer[250];
  sprintf(status_buffer, "{\
'state':%d,\
'emergency':%d,\
'blade_state':%d,\
'blade_temp': %.2f\
'rain':%d,\
'home':%d,\
'play':%d,\
'battery_v': %.2f, \
'charger_state:' %d,\
'charger_v': %.2f\
'charger_a': %.2f,\
}\n",status,EMERGENCY_State(),blademotor_eState,blade_temperature,RAIN_Sense(),BUTTON_Home(),BUTTON_Play(),battery_voltage,charger_state,chargerInputVoltage,current);
  
  
  
  logSerial((uint8_t *)status_buffer);

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
