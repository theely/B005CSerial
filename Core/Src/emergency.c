#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "emergency.h"
#include "main.h"


static uint8_t emergency_state = 0;
static uint32_t stop_emergency_started = 0;
static uint32_t wheel_lift_emergency_started = 0;
static uint32_t tilt_emergency_started = 0;
static uint32_t accelerometer_int_emergency_started = 0;
static uint32_t play_button_started = 0;

#define STOP_BUTTON_EMERGENCY_MILLIS 100
#define WHEEL_LIFT_EMERGENCY_MILLIS 1000
#define TILT_EMERGENCY_MILLIS 500

uint8_t EMERGENCY_State(void)
{
    return(emergency_state);
}


void  Emergency_SetState(uint8_t new_emergency_state)
{
    emergency_state = new_emergency_state;
}


int Emergency_Tilt(void)
{
   return(HAL_GPIO_ReadPin(Mechanical_Tilt_GPIO_Port, Mechanical_Tilt_Pin));
}



int Emergency_StopButtonYellow(void)
{
   return(HAL_GPIO_ReadPin(Stop_Button_Yellow_GPIO_Port, Stop_Button_Yellow_Pin));
}

int Emergency_StopButtonWhite(void)
{
    return(HAL_GPIO_ReadPin(Stop_Button_White_GPIO_Port, Stop_Button_White_Pin));
}



int Emergency_WheelLiftBlue(void)
{
   return(HAL_GPIO_ReadPin(Wheel_lift_blue_GPIO_Port, Wheel_lift_blue_Pin));
}



int Emergency_WheelLiftRed(void)
{
   return(HAL_GPIO_ReadPin(Wheel_lift_red_GPIO_Port, Wheel_lift_red_Pin));
}




void EMERGENCY_Update(void)
{
    uint8_t stop_button_yellow = Emergency_StopButtonYellow();
    uint8_t stop_button_white = Emergency_StopButtonWhite();
    uint8_t wheel_lift_blue = Emergency_WheelLiftBlue();
    uint8_t wheel_lift_red = Emergency_WheelLiftRed();
    uint8_t tilt = Emergency_Tilt();
    //TODO: GPIO_PinState play_button = !HAL_GPIO_ReadPin(PLAY_BUTTON_PORT, PLAY_BUTTON_PIN); // pullup, active low    
    //TODO: uint8_t accelerometer_int_triggered = Emergency_LowZAccelerometer();

    uint32_t now = HAL_GetTick();
 

    if (stop_button_yellow || stop_button_white)
    {
        if (stop_emergency_started == 0)
        {
            stop_emergency_started = now;
        }
        else
        {
            if (now - stop_emergency_started >= STOP_BUTTON_EMERGENCY_MILLIS)
            {
                if (stop_button_yellow)
                {
                    emergency_state |= 0b00010;
                    logSerial(" ## EMERGENCY ## - STOP BUTTON (yellow) triggered\r\n");
                }
                if (stop_button_white) {
                    emergency_state |= 0b00100;
                    logSerial(" ## EMERGENCY ## - STOP BUTTON (white) triggered\r\n");
                }
            }
        }
    }
    else
    {
        stop_emergency_started = 0;
    }

    if (wheel_lift_blue && wheel_lift_red)
    {
        if (wheel_lift_emergency_started == 0)
        {
            wheel_lift_emergency_started = now;
        }
        else
        {
            if (now - wheel_lift_emergency_started >= WHEEL_LIFT_EMERGENCY_MILLIS)
            {
                if (wheel_lift_blue)
                {
                    emergency_state |= 0b01000;
                    logSerial(" ## EMERGENCY ## - WHEEL LIFT (blue) triggered\r\n");
                }
                if (wheel_lift_red)
                {
                    emergency_state |= 0b10000;
                    logSerial(" ## EMERGENCY ## - WHEEL LIFT (red) triggered\r\n");
                }
            }
        }
    }
    /*TODO: if (accelerometer_int_triggered)
    {
        if(accelerometer_int_emergency_started == 0)
        {
            accelerometer_int_emergency_started = now;
        }
        else
        {
            if (now - accelerometer_int_emergency_started >= TILT_EMERGENCY_MILLIS) {
                emergency_state |= 0b100000;
                debug_printf(" ## EMERGENCY ## - ACCELEROMETER TILT triggered\r\n");
            }
        }     
    }*/
    //else
    //{
        accelerometer_int_emergency_started = 0;
    //}
    
    if (tilt)
    {
        if(tilt_emergency_started == 0)
        {
            tilt_emergency_started = now;
        }
        else
        {
            if (now - tilt_emergency_started >= TILT_EMERGENCY_MILLIS) {
                emergency_state |= 0b100000;
                logSerial(" ## EMERGENCY ## - MECHANICAL TILT triggered\r\n");
            }
        }
    }
    else
    {
        tilt_emergency_started = 0;
    }

    /*TODO: if (emergency_state && play_button)
    {
        if(play_button_started == 0)
        {
            play_button_started = now;
        }
        else
        {
            if (now - play_button_started >= PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS) {
                emergency_state = 0;
                debug_printf(" ## EMERGENCY ## - manual reset\r\n");
				StatusLEDUpdate();
                chirp(1);
            }
        }
    }
    else
    *///{
        play_button_started = 0;
    //}
}