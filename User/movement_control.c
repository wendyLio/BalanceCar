#include "movement_control.h"
#include "bsp_Timer_In_Out.h"
#include "stdlib.h"

u8 Direction_Left = 0,Rirection_Right = 0;

void Movement_Init(void)
{
	//初始化控制电机转向的4个GPIO 	(GPIOA_Pin_1 GPIOA_Pin_2 GPIOA_Pin_3 GPIOA_Pin_4)
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 //这里需要4个端口，要对着电路板查一下
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    	 //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
}

//正转:IN1 1 IN2 0
//反转:IN1 0 IN2 1
//停止:IN1 0 IN2 0
void Left_Wheel_Movement(u8 direction,u16 speed) //0 停止 1前进 2后退    speed取值0-1000
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
	LC_CHG(speed);   //F为需频率 C为占空比 如F=2000，C取值0-1000
}

void Right_Wheel_Movement(u8 direction,u16 speed) //0 停止 1前进 2后退    speed取值0-1000
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
	//车轮转向判断
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

void Get_Speed(u32 * Speed_Left_CM_S,u32 * Speed_Right_CM_S)	//这个速度从码盘获得，所以用u32表示，不涉及方向
{
	*Speed_Left_CM_S = Get_Left_Speed();	//获取转速
	*Speed_Right_CM_S = Get_Right_Speed();
}

void Get_Direction(u8 * Direction_Left_out,u8 * Direction_Right_out)	//获取方向，0停止 1向前 2向后
{
	(*Direction_Left_out)  = Direction_Left;
	(*Direction_Right_out) = Rirection_Right;
}

