#ifndef __BLADEMOTOR_H
#define __BLADEMOTOR_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#define BLADEMOTOR_LENGTH_RECEIVED_MSG 16

void BLADEMOTOR_Init(void);
void BLADEMOTOR_Run(void);
void BLADEMOTOR_UART_RxCallback(void);
void BLADEMOTOR_Set(uint8_t on_off);




#ifdef __cplusplus
}
#endif

#endif /*BLADEMOTOR_H*/ 