#include "movement_control.h"
#include "bsp_Timer_In_Out.h"
#include "stdlib.h"

u8 Direction_Left = 0,Rirection_Right = 0;

void Movement_Init(void)
{
	//��ʼ�����Ƶ��ת���4��GPIO 	(GPIOA_Pin_1 GPIOA_Pin_2 GPIOA_Pin_3 GPIOA_Pin_4)
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 //������Ҫ4���˿ڣ�Ҫ���ŵ�·���һ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    	 //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
}

//��ת:IN1 1 IN2 0
//��ת:IN1 0 IN2 1
//ֹͣ:IN1 0 IN2 0
void Left_Wheel_Movement(u8 direction,u16 speed) //0 ֹͣ 1ǰ�� 2����    speedȡֵ0-1000
{
	direction = 1;
	if(direction == 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
	if(direction == 1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
	else if(direction == 2)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		
	}
	
	speed = 500;
	LC_CHG(speed);   //FΪ��Ƶ�� CΪռ�ձ� ��F=2000��Cȡֵ0-1000
}

void Right_Wheel_Movement(u8 direction,u16 speed) //0 ֹͣ 1ǰ�� 2����    speedȡֵ0-1000
{
	direction = 1;
	if(direction == 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
	else if(direction == 1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
	else if(direction == 2)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
	else
	{
		
	}
	
	speed = 500;
	RC_CHG(speed);
}

void Speed_OutPut(s32 Speed_Left,s32 Speed_Right)
{
	//����ת���ж�
	if(Speed_Left > 0)
		Direction_Left = 1;
	else if(Speed_Left < 0)
		Direction_Left = 2;
	else 
		Direction_Left = 0;
	
	if(Speed_Right > 0)
		Rirection_Right = 1;
	else if(Speed_Right < 0)
		Rirection_Right = 2;
	else
		Rirection_Right = 0;
	
	Speed_Left = abs(Speed_Left);
	Speed_Right = abs(Speed_Right);
	
	Left_Wheel_Movement(Direction_Left,Speed_Left);
	Right_Wheel_Movement(Rirection_Right,Speed_Right);
}

void Get_Speed(u32 * Speed_Left_CM_S,u32 * Speed_Right_CM_S)	//����ٶȴ����̻�ã�������u32��ʾ�����漰����
{
	*Speed_Left_CM_S = Get_Left_Speed();	//��ȡת��
	*Speed_Right_CM_S = Get_Right_Speed();
}

void Get_Direction(u8 * Direction_Left_out,u8 * Direction_Right_out)	//��ȡ����0ֹͣ 1��ǰ 2���
{
	(*Direction_Left_out)  = Direction_Left;
	(*Direction_Right_out) = Rirection_Right;
}

