#ifndef _H_TIMER
#define _H_TIMER

#include "sys.h"

extern float ads8688_ch1, temp_A1, temp_A2;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);

#endif
