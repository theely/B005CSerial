
#include <string.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#include "main.h"
#include "usart.h"
#include "blademotor.h" 

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define BLADEMOTOR_LENGTH_INIT_MSG 22
#define BLADEMOTOR_LENGTH_RQST_MSG 7
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum {
    BLADEMOTOR_INIT_1,
    BLADEMOTOR_INIT_2,
    BLADEMOTOR_RUN
}BLADEMOTOR_STATE_e;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static BLADEMOTOR_STATE_e blademotor_eState = BLADEMOTOR_INIT_1;



static uint8_t blademotor_pu8ReceivedData[BLADEMOTOR_LENGTH_RECEIVED_MSG] = {0};
static uint8_t blademotor_pu8RqstMessage[BLADEMOTOR_LENGTH_RQST_MSG]  = {0x55, 0xaa, 0x03, 0x20, 0x80, 0x00, 0xA2};

const uint8_t blademotor_pcu8Preamble[5]  = {0x55,0xAA,0x0A,0x2,0xD0};
const uint8_t blademotor_pcu8InitMsg[BLADEMOTOR_LENGTH_INIT_MSG] =  { 0x55, 0xaa, 0x12, 0x20, 0x80, 0x00, 0xac, 0x0d, 0x00, 0x02, 0x32, 0x50, 0x1e, 0x04, 0x00, 0x15, 0x21, 0x05, 0x0a, 0x19, 0x3c, 0xaa };
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
*  Public Functions
*******************************************************************************/

/**
 * @brief Init the Blade Motor Serial Port (PAC5223)
 * @retval None
 */
void BLADEMOTOR_Init(void)
{

    /* PAC 5223 Reset Line (Blade Motor) */
    HAL_GPIO_WritePin(Blade_Motor_Reset_GPIO_Port, Blade_Motor_Reset_Pin, 1);     /* take Blade PAC out of reset if HIGH */

    blademotor_eState = BLADEMOTOR_INIT_1;    
}

/// @brief handle drive motor messages
/// @param  
void  BLADEMOTOR_Run(void){
    switch (blademotor_eState)
    {
    case BLADEMOTOR_INIT_1:

        HAL_UART_Transmit_DMA(&huart6, (uint8_t*)blademotor_pcu8InitMsg, BLADEMOTOR_LENGTH_INIT_MSG);
        blademotor_eState = BLADEMOTOR_RUN;
        logSerial(" * Blade Motor Controller initialized\r\n");     
        break;
    
    case BLADEMOTOR_RUN:

        HAL_UART_Transmit_DMA(&huart6, (uint8_t*)blademotor_pu8RqstMessage, BLADEMOTOR_LENGTH_RQST_MSG);    
        break;
    
    default:
        break;
    }
}

/// @brief control blade motor (there is no speed control for this motor)
/// @param on_off 1 to turn on, 0 to turn off
void BLADEMOTOR_Set(uint8_t on_off)
{    
    if (on_off)
    {
        blademotor_pu8RqstMessage[5] = 0x80; /* change speed Motor */
        blademotor_pu8RqstMessage[6] = 0x22; /* change CRC */
    }
    else
    {
        blademotor_pu8RqstMessage[5] = 0x00; /* change speed Motor */
        blademotor_pu8RqstMessage[6] = 0xa2; /* change CRC */
    }
}

/// @brief drive motor receive interrupt handler
/// @param  
void BLADEMOTOR_UART_RxCallback(void)
{
    //receive packet  
    HAL_UART_Receive_DMA(&huart6, blademotor_pu8ReceivedData, BLADEMOTOR_LENGTH_RECEIVED_MSG);
    /* decode the frame */    
    if(memcmp(blademotor_pcu8Preamble, blademotor_pu8ReceivedData, 2) == 0){        
        uint8_t l_u8crc = crcCalc(blademotor_pu8ReceivedData, BLADEMOTOR_LENGTH_RECEIVED_MSG-1);

        if(blademotor_pu8ReceivedData[BLADEMOTOR_LENGTH_RECEIVED_MSG-1] == l_u8crc ){

            logSerial("Blade motor packet ok\n");


            if((blademotor_pu8ReceivedData[5] & 0x80) == 0x80){
                //BLADEMOTOR_bActivated = true;
            }
            else{
                //BLADEMOTOR_bActivated = false;
            }
            //BLADEMOTOR_u16RPM = blademotor_pu8ReceivedData[7] + (blademotor_pu8ReceivedData[8]<<8);
            //BLADEMOTOR_u16Power = blademotor_pu8ReceivedData[9] + (blademotor_pu8ReceivedData[10]<<8) ;           
        }
  
    }
}

/******************************************************************************
*  Private Functions
*******************************************************************************/