#ifndef __TIME_H
#define	__TIME_H

#include "stm32f10x.h"

extern int go_onflag;

void delay_us(u32 us);
void Delay_Ms(u32 myMs);
void Delay_Us(u32 myUs);
void TIM7_Configuration(void);
void Delay(int delaytime);

#endif /* __TMIE_H */
