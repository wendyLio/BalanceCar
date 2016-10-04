#include "scheduler.h"
#include "attitude_computation.h"
#include "stdio.h"

void Duty_1ms(void);							//����1ms������
void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);

loop_t loop;

//systick���Ƶĵȴ�������ֹͣ�������񣬽��еȴ�
//�˺���ֻ��systick��ʼ������Ч
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
		//���д����ʵ������ʡʱ�䣬�����ڶ�������������Ӧ�ñȽϿ�
		//���ʱ�䲻���˿��԰��ⲿ�ָĵ����б�����򵥵ö��д��
		//�õ�һ����������+ȡ�����ķ�������
		loop.cnt_2ms++;
		loop.cnt_5ms++;
		loop.cnt_10ms++;
		loop.cnt_20ms++;
		loop.cnt_50ms++;
		
		if(loop.check_flag == 1)	//���������check_flag�����ﻹ��1��û�б���0��
									//֤����ѭ������1ms������û�������꣬������check_flagû�����е�
		{
			loop.error_flag++;		//ÿ�γ������⣬error_flag+1
		}
		
		loop.check_flag = 1;
		
		//��仰Ӧ�ò��������ã�������ǿ³���԰�
		//���delay_ms_counter<0�ˣ�������ʱ�߼��ͳ���������
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
		Duty_1ms();							//����1ms������
		
		if(loop.cnt_2ms >= 2)
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}
		
		loop.check_flag = 0;	//�����������
	}
}

void Duty_1ms(void)
{
	
}

void Duty_2ms(void)
{
	u32 T = 2;
	Attitude_sensor_Read(T);
}

void Duty_5ms(void)
{
	u32 T = 5;
	Attitude_sensor_Update(T);
	
	//����������������ݲ���������������ȷ��
	printf("GYRO:%f %f %f; Angle:%f %f %f\n",Gyro.x,Gyro.y,Gyro.z,Angle.x,Angle.y,Angle.z);
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


