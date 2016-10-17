#include "time.h"

/*
使用需求：1.独立的延时函数，能够实现delay_us




*/

//while循环实现

int go_onflag=0;
void Delay_Us(u32 myUs)   
{
  u32 i;
  while(myUs--)
  {
    i=6;
    while(i--);
  }
}
 
void Delay_Ms(u32 myMs)
{
  u32 i;
  while(myMs--)
  {
    i=7200;
    while(i--);
  }
}

void delay_us(u32 us)
{
	Delay_Us(us);   
}


void TIM7_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 50000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM4,DISABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, DISABLE);
}

void Delay(int delaytime)
{   
    TIM_SetCounter(TIM4,0);
	TIM_SetAutoreload(TIM4,delaytime);
    TIM_Cmd(TIM4, ENABLE);
    while(go_onflag==0);
    TIM_Cmd(TIM4, DISABLE);
    go_onflag=0;
}
