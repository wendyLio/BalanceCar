#ifndef __BSP_ENCODER_H
#define	__BSP_ENCODER_H

#include "stm32f10x.h"
#include "include.h"

void Encoder_Configuration(void);

u16 Get_Left_Speed(void);	//��ȡת��
u16 Get_Right_Speed(void);

#endif /* __BSP_ENCODER_H */
