#include "scheduler.h"

void Duty_1ms(void);							//周期1ms的任务
void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);

loop_t loop;

//systick控制的等待函数，停止所有任务，进行等待
//此函数只在systick初始化后有效
int delay_ms_counter = 0;
void Delay_ms(int nms)
{
	delay_ms_counter = nms;
	while(delay_ms_counter);
}

void Loop_check(void)  //TIME INTTERRUPT
{
	if(delay_ms_counter != 0)
	{
		delay_ms_counter--;
	}
	else
	{
		//这个写法其实并不节省时间，但好在都是整数操作，应该比较快
		//如果时间不够了可以把这部分改掉，有比这个简单得多的写法
		//用单一计数器变量+取余数的方法就行
		loop.cnt_2ms++;
		loop.cnt_5ms++;
		loop.cnt_10ms++;
		loop.cnt_20ms++;
		loop.cnt_50ms++;
		
		if(loop.check_flag == 1)	//如果到这里check_flag到这里还是1，没有被清0，
									//证明主循环里面1ms的任务没有运行完，最后面的check_flag没有运行到
		{
			loop.error_flag++;		//每次出现问题，error_flag+1
		}
		
		loop.check_flag = 1;
		
		//这句话应该不会起作用，算是增强鲁棒性吧
		//如果delay_ms_counter<0了，整个延时逻辑就出大问题了
		if(delay_ms_counter < 0)
			delay_ms_counter = 0;
	}
}

void Loop_Init(void)
{
	loop.check_flag = 0;
	
	loop.cnt_2ms = 0;
	loop.cnt_5ms = 0;
	loop.cnt_10ms = 0;
	loop.cnt_20ms = 0;
	loop.cnt_50ms = 0;
	
	loop.error_flag = 0;
}

void Duty_Loop(void)
{
	if(loop.check_flag == 1)
	{
		Duty_1ms();							//周期1ms的任务
		
		if(loop.cnt_2ms >= 2)
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//周期2ms的任务
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//周期10ms的任务
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//周期20ms的任务
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//周期50ms的任务
		}
		
		loop.check_flag = 0;	//运行完毕清零
	}
}

void Duty_1ms(void)
{
	
}

void Duty_2ms(void)
{
	
}

void Duty_5ms(void)
{
	
}

void Duty_10ms(void)
{
	
}

void Duty_20ms(void)
{
	
}

void Duty_50ms(void)
{
	
}

