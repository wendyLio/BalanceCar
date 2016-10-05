#ifndef __CONTROL_H
#define	__CONTROL_H

#include "stm32f10x.h"

extern u32 kp,kd;
extern s32 Control_Out_Left,Control_Out_Right;

void Balance_Init(void);
void Balance_Control(float Angle_y,float Gyro_y,float Expect_Angle_y);


#endif /* __CONTROL_H */
