#include "scheduler.h"
#include "attitude_computation.h"
#include "control.h"
#include "movement_control.h"
#include "stdio.h"
#include "time.h"

void Duty_1ms(void);							//����1ms������
void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);

loop_t loop;

void Loop_check(void)  //TIME INTTERRUPT
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
	Attitude_sensor_Read(T);	//��ȡ���ݣ���ͨ�˲�
}

void Duty_5ms(void)
{
//	static int i = 0;
	u32 T = 5;
	s16 Control_Out_Left;
	s16 Control_Out_Right;
	
	Attitude_sensor_Update(T);	//��̬���ݸ���
	Balance_Control(Angle.y,Gyro.y,&Control_Out_Left,&Control_Out_Right,0.0f);	//ƽ��PID����
	Speed_OutPut(Control_Out_Left,Control_Out_Right);	//�����������ֵ��ֵ�������������
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


//	u32 a,b;

	//����������������ݲ���������������ȷ��
//	i++;
//	if(i>7)
//	{
//		i = 0;
////		printf("GYRO:%f %f %f; Angle:%f %f %f\n",Gyro.x,Gyro.y,Gyro.z,Angle.x,Angle.y,Angle.z);
////		printf("G:%f	Angle:%f\n",Gyro.y,Angle.y);
//	}

//	printf("a:%d	b:%d\n",a,b);

//	Get_Speed(&a,&b);
//	a = readnowtime();
//	i++;
//	if(i>20)
//	{
//		i = 0;
//		printf("a:%d\n",a);
//	}
