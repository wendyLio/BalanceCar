#ifndef __MOVEMENT_CONTROL_H
#define	__MOVEMENT_CONTROL_H

#include "stm32f10x.h"

//extern u8 Direction_Left,Rirection_Right;	//1ǰ����2���ˣ�0ֹͣ   ���ú�����ʽ�ṩ�����ˣ���������ֱ�ӷ���

void Speed_InPut(s32 Speed_Left,s32 Speed_Right);

void Get_Speed(u32 * Speed_Left_CM_S,u32 * Speed_Right_CM_S);	//����ٶȴ����̻�ã�������u32��ʾ�����漰����
void Get_Direction(u8 * Direction_Left_out,u8 * Direction_Right_out);	//��ȡ����0ֹͣ 1��ǰ 2���

#endif /* __MOVEMENT_CONTROL_H */