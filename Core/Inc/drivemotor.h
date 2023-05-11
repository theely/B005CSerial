
#ifndef __DRIVEMOTOR_H
#define __DRIVEMOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern int16_t    right_wheel_speed_val;
extern int16_t    left_wheel_speed_val;
extern uint32_t   right_encoder_ticks;  // accumulating
extern uint32_t   left_encoder_ticks;   // accumulating 
extern uint16_t   right_encoder_val;    // non accumulating 
extern uint16_t   left_encoder_val;     // non accumulating 
extern uint8_t    right_power;
extern uint8_t    left_power;
extern uint32_t   DRIVEMOTOR_u32ErrorCnt;




typedef enum {
    RX_WAIT,
    RX_VALID,
    RX_CRC_ERROR,
    RX_INVALID_ERROR,
    RX_TIMEOUT_ERROR,
}rx_status_e;


#define MAX_MPS 0.5		  // Allow maximum speed of 0.5 m/s
#define PWM_PER_MPS 300.0 // PWM value of 300 means 1 m/s bot speed

#define TICKS_PER_M 300.0 // Motor Encoder ticks per meter

// #define WHEEL_BASE  0.325		// The distance between the center of the wheels in meters
#define WHEEL_BASE 0.285   // The distance between the center of the wheels in meters
#define WHEEL_DIAMETER 0.2 // The diameter of the wheels in meters



void DRIVEMOTOR_Init(void);
void DRIVEMOTOR_Run(void);
void DRIVEMOTOR_Receive(void);
void DRIVEMOTOR_UART_RxCallback(void);
void DRIVEMOTOR_SetSpeed(float left_mps, float right_mps);
uint8_t crcCalc(uint8_t *msg, uint8_t msg_len);
#ifdef __cplusplus
}
#endif
#endif /*__DRIVEMOTOR_H*/ 