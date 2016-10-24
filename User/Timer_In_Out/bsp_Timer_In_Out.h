#ifndef __BSP_ENCODER_H
#define	__BSP_ENCODER_H

#include "stm32f10x.h"
#include "include.h"

void TIM_Input_Output_Configuration(void);

void LC_CHG(u16 ChannelPulse);   //F为需频率 C为占空比 如F=2000，C取值0-1000
void RC_CHG(u16 ChannelPulse);

u16 Get_Left_Speed(void);	//获取转速
u16 Get_Right_Speed(void);

#endif /* __BSP_ENCODER_H */
