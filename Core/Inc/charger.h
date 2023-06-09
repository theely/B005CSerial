#ifndef __CHARGER_H
#define __CHARGER_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* Includes
*******************************************************************************/

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/
#define MIN_DOCKED_VOLTAGE 20.0f
#define MAX_CHARGE_CURRENT 1.0f
/// limite voltag when switching in 150mA mode
#define LIMIT_VOLTAGE_150MA 28.0f
/// Max voltage allowed 29.4
#define MAX_CHARGE_VOLTAGE 29.0f
/// Max battery voltage allowed
#define BAT_CHARGE_CUTOFF_VOLTAGE 28.0f

//About 60%-70% od SOC
#define BAT_CHARGE_STORAGE_VOLTAGE 26.0f


/// We consider the battery is full when in CV mode the current below 0.1A
#define CHARGE_END_LIMIT_CURRENT 0.08f
// if voltage is greater than this assume we are docked
#define MIN_DOCKED_VOLTAGE 20.0f
// if voltage is lower this assume battery is disconnected
#define MIN_BATTERY_VOLTAGE 5.0f

// if current is greater than this assume the battery is charging
#define MIN_CHARGE_CURRENT 0.1f
#define LOW_BAT_THRESHOLD 25.2f /* near 20% SOC */
#define LOW_CRI_THRESHOLD 23.5f /* near 0% SOC */
/******************************************************************************
* Typedefs
*******************************************************************************/

typedef enum{
    CHARGER_STATE_OFF=0,
    CHARGER_STATE_IDLE,
    CHARGER_STATE_CONNECTED,
    CHARGER_STATE_CHARGING_CC,
    CHARGER_STATE_CHARGING_CV,
    CHARGER_STATE_END_CHARGING,
} CHARGER_STATE_e;
/******************************************************************************
* Variables
*******************************************************************************/
//extern float SOC; needs refactoring
extern uint16_t chargecontrol_pwm_val;
extern CHARGER_STATE_e charger_state;

/******************************************************************************
* PUBLIC Function Prototypes
*******************************************************************************/

void CHARGER_Init(void);
void CHARGER_Update(void);
void CHARGER_Set(uint8_t on_off);


#ifdef __cplusplus
}
#endif
#endif /*__CHARGER_H*/ 

/*** End of File **************************************************************/