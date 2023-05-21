
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "rtc.h"
#include "charger.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

typedef enum{
    CHARGER_STATE_IDLE,
    CHARGER_STATE_CONNECTED,
    CHARGER_STATE_CHARGING_CC,
    CHARGER_STATE_CHARGING_CV,
    CHARGER_STATE_END_CHARGING,
} CHARGER_STATE_e;

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/


float SOC                           = 0;
uint16_t chargecontrol_pwm_val      = 0;
uint8_t  chargecontrol_is_charging  = 0;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 *  Public Functions
 *******************************************************************************/


void CHARGER_Init(void){
  TIM1->CCR1 = 0;  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
}



void CHARGER_Update(void)
{                        
  static CHARGER_STATE_e charger_state = CHARGER_STATE_IDLE;
  static uint32_t timestamp = 0;

  /*charger disconnected force idle state*/
  if(( chargerInputVoltage < MIN_DOCKED_VOLTAGE) ){
    charger_state = CHARGER_STATE_IDLE;
  }
    
    switch (charger_state)
    {
    case CHARGER_STATE_CONNECTED:
        
        /* when connected the 3.3v and 5v is provided by the charger so we get the real biais of the current measure */
        chargecontrol_pwm_val = 0;

        /* wait 100ms to read current */
        if( (HAL_GetTick() - timestamp) > 100){
          charge_current_offset.f = current_without_offset;
          // Writes a data in a RTC Backup data Register 3&4
          HAL_PWR_EnableBkUpAccess();
          HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, charge_current_offset.u[0]);    
          HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, charge_current_offset.u[1]);   
          HAL_PWR_DisableBkUpAccess(); 
          HAL_GPIO_WritePin(High_Voltage_Enable_GPIO_Port, High_Voltage_Enable_Pin, 1); /* Power on the battery  Powerbus */
          charger_state = CHARGER_STATE_CHARGING_CC;
        }

        break;

    case CHARGER_STATE_CHARGING_CC:
        // cap charge current at 1.5 Amps
        if ((battery_voltage > BAT_CHARGE_CUTOFF_VOLTAGE && (chargecontrol_pwm_val > 0)) || ((current > MAX_CHARGE_CURRENT) && (chargecontrol_pwm_val > 50)))
        {
            chargecontrol_pwm_val--;
        }
        if ((battery_voltage < BAT_CHARGE_CUTOFF_VOLTAGE) && (current < MAX_CHARGE_CURRENT) && (chargecontrol_pwm_val < 1350))
        {
            chargecontrol_pwm_val++;
        }

        if(charge_voltage >= (LIMIT_VOLTAGE_150MA )){
            charger_state = CHARGER_STATE_CHARGING_CV;
        }

        break;

    case CHARGER_STATE_CHARGING_CV:
        // set PWM to approach 29.4V  charge voltage
        if ((battery_voltage < BAT_CHARGE_CUTOFF_VOLTAGE) && (charge_voltage < (MAX_CHARGE_VOLTAGE)) && (chargecontrol_pwm_val < 1350))
        {
          chargecontrol_pwm_val++;
        }            
        if ((battery_voltage > BAT_CHARGE_CUTOFF_VOLTAGE && (chargecontrol_pwm_val > 0)) || (charge_voltage > (MAX_CHARGE_VOLTAGE) && (chargecontrol_pwm_val > 50)))
        {
          chargecontrol_pwm_val--;
        }

        /* the current is limited to 150ma */
        if ((current > (MAX_CHARGE_CURRENT/10)) && chargecontrol_pwm_val > 0)
        {
            chargecontrol_pwm_val--;
        }

        /* battery full ? */
        if (current < CHARGE_END_LIMIT_CURRENT) {
          //charger_state = CHARGER_STATE_END_CHARGING;
          /*consider as the battery full */
          ampere_acc.f = 2.8;
          SOC = 100;
        }

        break;

    case CHARGER_STATE_END_CHARGING:

        chargecontrol_pwm_val = 0;

        break;


    case CHARGER_STATE_IDLE:
    default:
       
        if (chargerInputVoltage >= 30.0 ) {
            charger_state = CHARGER_STATE_CONNECTED;
            HAL_GPIO_WritePin(High_Voltage_Enable_GPIO_Port, High_Voltage_Enable_Pin, 0); /* Power off the battery  Powerbus */
            timestamp = HAL_GetTick();
        }
        chargecontrol_pwm_val = 0;
        break;
    }
    
    ampere_acc.f += ((current - charge_current_offset.f)/(100*60*60));
    if(ampere_acc.f >= 2.8)ampere_acc.f = 2.8;
    SOC = ampere_acc.f/2.8;

    // Writes a data in a RTC Backup data Register 1
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, ampere_acc.u[0]);    
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, ampere_acc.u[1]);   
    HAL_PWR_DisableBkUpAccess(); 

    chargecontrol_is_charging = charger_state;

    /*Check the PWM value for safety */
    if (chargecontrol_pwm_val > 1350){
        chargecontrol_pwm_val = 1350;
    }
    TIM1->CCR1 = chargecontrol_pwm_val;  
    
}

/******************************************************************************
 *  Private Functions
 *******************************************************************************/
