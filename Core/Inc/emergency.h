
#ifndef __EMERGENCY_H
#define __EMERGENCY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ACK_TIMEOUT 500

uint8_t EMERGENCY_State(void);
void Emergency_SetState(uint8_t new_emergency_state);
int Emergency_Tilt(void);
int Emergency_StopButtonYellow(void);
int Emergency_StopButtonWhite(void);
int Emergency_WheelLiftBlue(void);
int Emergency_WheelLiftRed(void);
int Emergency_LowZAccelerometer(void);
void EMERGENCY_Update(void);
void EMERGENCY_SerialAck(void);
void EMERGENCY_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__EMERGENCY_H*/ 