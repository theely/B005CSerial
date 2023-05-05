/******************************************************************************
* Includes
*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#include "main.h"
#include "usart.h"
#include "drivemotor.h" 

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define DRIVEMOTOR_LENGTH_INIT_MSG 38
#define DRIVEMOTOR_LENGTH_RQST_MSG 12
#define DRIVEMOTOR_LENGTH_RECEIVED_MSG 20
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum {
    DRIVEMOTOR_INIT_1,
    DRIVEMOTOR_INIT_2,
    DRIVEMOTOR_RUN,
    DRIVEMOTOR_BACKWARD,
    DRIVEMOTOR_WAIT
}DRIVEMOTOR_STATE_e;



typedef struct 
{
    /* 0*/ uint16_t u16_preambule;
    /* 2*/ uint8_t u8_length;
    /* 3*/ uint16_t u16_id;
    /* 5*/ uint8_t u8_direction;
    /* 6*/ uint8_t u8_left_speed;
    /* 7*/ uint8_t u8_right_speed;
    /* 8*/ uint16_t u16_ukndata0;
    /*10*/ uint8_t u8_left_power;
    /*11*/ uint8_t u8_right_power;
    /*12*/ uint8_t u8_error;
    /*13*/ uint16_t u16_left_ticks;
    /*15*/ uint16_t u16_right_ticks;
    /*17*/ uint8_t u8_left_ukn;
    /*18*/ uint8_t u8_right_ukn;
    /*19*/ uint8_t u8_CRC;
} __attribute__((__packed__))DRIVEMOTORS_data_t;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


static DRIVEMOTOR_STATE_e drivemotor_eState = DRIVEMOTOR_INIT_1;
static rx_status_e drivemotors_eRxFlag = RX_WAIT;

static DRIVEMOTORS_data_t drivemotor_psReceivedData = {0};
static uint8_t drivemotor_pu8RqstMessage[DRIVEMOTOR_LENGTH_RQST_MSG]  = { 0x55, 0xaa, 0x08, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t drivemotor_pcu8Preamble[5]  = {0x55,0xAA,0x10,0x01,0xE0};
//const uint8_t drivemotor_pcu8InitMsg[DRIVEMOTOR_LENGTH_INIT_MSG] = { 0x55, 0xaa, 0x08, 0x10, 0x80, 0xa0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x37};
const uint8_t drivemotor_pcu8InitMsg[DRIVEMOTOR_LENGTH_INIT_MSG] =  { 0x55, 0xaa, 0x22, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0xC8, 0x46, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0x14, 0x96, 0x0A, 0x1E, 0x5a, 0xfa, 0x05, 0x0A, 0x14, 0x32, 0x40, 0x04, 0x20, 0x01, 0x00, 0x00, 0x2C, 0x01, 0xEE};


int8_t prev_left_direction = 0;
int8_t prev_right_direction = 0;
uint16_t prev_right_encoder_val = 0;
uint16_t prev_left_encoder_val = 0;
int16_t prev_right_wheel_speed_val = 0;
int16_t prev_left_wheel_speed_val = 0;
uint32_t right_encoder_ticks = 0;
uint32_t left_encoder_ticks = 0;
int8_t left_direction = 0;
int8_t right_direction = 0;
uint16_t right_encoder_val = 0;
uint16_t left_encoder_val = 0;
int16_t right_wheel_speed_val = 0;
int16_t left_wheel_speed_val = 0;
uint8_t right_power = 0;
uint8_t left_power = 0;



uint32_t DRIVEMOTOR_u32ErrorCnt = 0;

static uint8_t left_speed_req;
static uint8_t right_speed_req;
static uint8_t left_dir_req;
static uint8_t right_dir_req;
    
/******************************************************************************
* Function Prototypes
*******************************************************************************/
__STATIC_INLINE  void drivemotor_prepareMsg(uint8_t left_speed, uint8_t right_speed, uint8_t left_dir, uint8_t right_dir);

/******************************************************************************
*  Public Functions
*******************************************************************************/

/// @brief Initialize STM32 hardware UART to control drive motors
/// @param  
void DRIVEMOTOR_Init(void){


   HAL_GPIO_WritePin(Driver_Motor_Enable_GPIO_Port, Driver_Motor_Enable_Pin, 0);     // take Drive Motor PAC out of reset if LOW
   HAL_GPIO_WritePin(Driver_Motor_EnableD7_GPIO_Port, Driver_Motor_EnableD7_Pin  , 1);
   HAL_GPIO_WritePin(Driver_Motor_EnableD8_GPIO_Port, Driver_Motor_EnableD8_Pin , 1);


    right_encoder_ticks = 0;
    left_encoder_ticks = 0;
    prev_left_direction = 0;
    prev_right_direction = 0;
    prev_right_encoder_val = 0;
    prev_left_encoder_val = 0;
    prev_right_wheel_speed_val = 0;
    prev_left_wheel_speed_val = 0;
}

/// @brief handle drive motor messages
/// @param  
void DRIVEMOTOR_App_10ms(void){

    static uint32_t l_u32Timestamp = 0;

    switch (drivemotor_eState)
    {
        case DRIVEMOTOR_INIT_1:

            HAL_UART_Transmit_DMA(&huart2, (uint8_t*)drivemotor_pcu8InitMsg, DRIVEMOTOR_LENGTH_INIT_MSG);
            drivemotor_eState = DRIVEMOTOR_RUN;
            logSerial("Drive Motor Controller initialized\r\n");        
            break;
        
        case DRIVEMOTOR_RUN:
            
            /* prepare to receive the message before to launch the command */
            HAL_UART_Receive_DMA(&huart2, (uint8_t*)&drivemotor_psReceivedData, sizeof(DRIVEMOTORS_data_t));

            drivemotor_prepareMsg(left_speed_req, right_speed_req, left_dir_req, right_dir_req);
            /* error State*/
            if(drivemotor_psReceivedData.u8_error != 0){
                drivemotor_prepareMsg(0,0,0,0);
                DRIVEMOTOR_u32ErrorCnt++;
                logSerial("Drive Motor Error\r\n"); 
            }            

            /* hit an obstacle  and move forward!!!!!! stop and wait*/
            //if((HALLSTOP_Left_Sense() || HALLSTOP_Right_Sense()) && (left_dir_req || right_dir_req)  ){
            //    drivemotor_prepareMsg(0,0,0,0);
            //}



            HAL_UART_Transmit_DMA(&huart2, (uint8_t*)drivemotor_pu8RqstMessage, DRIVEMOTOR_LENGTH_RQST_MSG);

            break;

        case DRIVEMOTOR_WAIT:  //TODO: check why this never happens
            /* prepare to receive the message before to launch the command */
            HAL_UART_Receive_DMA(&huart2, (uint8_t*)&drivemotor_psReceivedData, sizeof(DRIVEMOTORS_data_t));
            drivemotor_prepareMsg(0,0,0,0);
            if( (HAL_GetTick() - l_u32Timestamp) > 1000){
                drivemotor_eState = DRIVEMOTOR_RUN;
            }
            HAL_UART_Transmit_DMA(&huart2, (uint8_t*)drivemotor_pu8RqstMessage, DRIVEMOTOR_LENGTH_RQST_MSG);

            break;
        
        default:
            break;
    }

}

/// @brief Decode received drive motor messages 
/// @param  
void DRIVEMOTOR_App_Rx(void){
    if(drivemotors_eRxFlag == RX_VALID )
    {
            /* decode */
            uint8_t direction = drivemotor_psReceivedData.u8_direction;
            // we need to adjust for direction (+/-) !
            if ((direction & 0xc0) == 0xc0)
            {            
                left_direction = 1;
            }
            else if ((direction & 0x80) == 0x80)
            {
                left_direction = -1;
            }
            else
            {
                left_direction = 0;
            }
            if ( (direction & 0x30) == 0x30)
            {            
                right_direction = 1;
            }
            else if ( (direction & 0x20) == 0x20)
            {
                right_direction = -1;
            }
            else
            {
                right_direction = 0;
            }    

                left_encoder_val = drivemotor_psReceivedData.u16_left_ticks;
            right_encoder_val = drivemotor_psReceivedData.u16_right_ticks;
    
            // power consumption
            left_power = drivemotor_psReceivedData.u8_left_power;
            right_power = drivemotor_psReceivedData.u8_right_power;         
            
            /*
              Encoder value can reset to zero twice when changing direction
              2nd reset occurs when the speed changes from zero to non-zero
              something the ticks are holded until the next commands
            */
           
            left_wheel_speed_val = left_direction * drivemotor_psReceivedData.u8_left_speed;
            if( left_direction == 0 || (left_direction != prev_left_direction) || (prev_left_wheel_speed_val == 0 && left_wheel_speed_val != 0) )
            {
                prev_left_encoder_val = 0;
            }
            left_encoder_ticks +=  abs(left_direction* (left_encoder_val - prev_left_encoder_val));
            prev_left_encoder_val = left_encoder_val;
            prev_left_wheel_speed_val = left_wheel_speed_val;
            prev_left_direction = left_direction;

            right_wheel_speed_val =  right_direction * drivemotor_psReceivedData.u8_right_speed;
            if( right_direction == 0 || (right_direction != prev_right_direction) || (prev_right_wheel_speed_val == 0 && right_wheel_speed_val != 0) )
            {
                prev_right_encoder_val = 0;
            }
            right_encoder_ticks +=  abs(right_direction*(right_encoder_val - prev_right_encoder_val));
            prev_right_encoder_val = right_encoder_val;
            prev_right_wheel_speed_val = right_wheel_speed_val;
            prev_right_direction = right_direction;

            //TODO: migh want to add this
            //wheelTicks_handler(left_direction, right_direction, left_encoder_ticks, right_encoder_ticks, left_wheel_speed_val, right_wheel_speed_val);
            
            drivemotors_eRxFlag = RX_WAIT;                    // ready for next message      

    }
}



void DRIVEMOTOR_SetSpeed(float left_mps, float right_mps)
{
    

	// cap left motor speed to MAX_MPS
	if (left_mps > MAX_MPS)
	{
		left_mps = MAX_MPS;
	}
	else if (left_mps < -1. * MAX_MPS)
	{
		left_mps = -1. * MAX_MPS;
	}
	// cap right motor speed to MAX_MPS
	if (right_mps > MAX_MPS)
	{
		right_mps = MAX_MPS;
	}
	else if (right_mps < -1. * MAX_MPS)
	{
		right_mps = -1. * MAX_MPS;
	}

    left_speed_req  = abs(left_mps * PWM_PER_MPS);
    right_speed_req = abs(right_mps * PWM_PER_MPS);
    left_dir_req    = (left_mps >= 0) ? 1 : 0;
    right_dir_req   = (right_mps >= 0) ? 1 : 0;
}



/// @brief drive motor receive interrupt handler
/// @param  
void DRIVEMOTOR_ReceiveIT(void)
{    
    /* decode the frame */
    if(memcmp(drivemotor_pcu8Preamble,(uint8_t*)&drivemotor_psReceivedData,5) == 0){
        uint8_t l_u8crc = crcCalc((uint8_t*)&drivemotor_psReceivedData,DRIVEMOTOR_LENGTH_RECEIVED_MSG-1);
        if(drivemotor_psReceivedData.u8_CRC == l_u8crc )
        {
            drivemotors_eRxFlag = RX_VALID;                  
        }
        else
        {
            drivemotors_eRxFlag = RX_CRC_ERROR; 
        }
    }
    else
    {
         drivemotors_eRxFlag = RX_INVALID_ERROR;
    }  
}

/******************************************************************************
*  Private Functions
*******************************************************************************/

__STATIC_INLINE  void  drivemotor_prepareMsg(uint8_t left_speed, uint8_t right_speed, uint8_t left_dir, uint8_t right_dir){

     uint8_t direction = 0x0;            

    // calc direction bits
     if (right_dir == 1)
    {        
        direction |= (0x20 + 0x10);
    }
    else
    {
        direction |= 0x20;
    }
    if (left_dir == 1)
    {   
        direction |= (0x40 + 0x80);                             
    }
    else
    {
        direction |= 0x80;
    }

    drivemotor_pu8RqstMessage[0]= 0x55 ;
    drivemotor_pu8RqstMessage[1]= 0xaa ;
    drivemotor_pu8RqstMessage[2]= 0x08 ;
    drivemotor_pu8RqstMessage[3]= 0x10 ;
    drivemotor_pu8RqstMessage[4]= 0x80 ;
    drivemotor_pu8RqstMessage[5] = direction;
    drivemotor_pu8RqstMessage[6] = left_speed;
    drivemotor_pu8RqstMessage[7] = right_speed;
    drivemotor_pu8RqstMessage[9]= 0;
    drivemotor_pu8RqstMessage[8] = 0;
    drivemotor_pu8RqstMessage[10] = 0;
    drivemotor_pu8RqstMessage[11] = crcCalc(drivemotor_pu8RqstMessage, DRIVEMOTOR_LENGTH_RQST_MSG-1);
}

uint8_t crcCalc(uint8_t *msg, uint8_t msg_len)
{
  uint8_t crc = 0x0;
  uint8_t i;

  for (i = 0; i < msg_len; i++)
  {
    crc += msg[i];
  }
  return (crc);
}