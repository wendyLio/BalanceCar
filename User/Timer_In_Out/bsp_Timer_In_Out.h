#ifndef __BSP_ENCODER_H
#define	__BSP_ENCODER_H

#include "stm32f10x.h"
#include "include.h"

void TIM_Input_Output_Configuration(void);

void LC_CHG(u16 ChannelPulse);   //FΪ��Ƶ�� CΪռ�ձ� ��F=2000��Cȡֵ0-1000
void RC_CHG(u16 ChannelPulse);

u16 Get_Left_Speed(void);	//��ȡת��
u16 Get_Right_Speed(void);

#endif /* __BSP_ENCODER_H */
