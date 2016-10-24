#include "stm32f10x.h"
#include "init.h"
#include "ioi2c.h"
#include "bsp_usart2.h"
#include "time.h"
#include "bsp_Timer_In_Out.h"


u8 All_Init(void)
{
	int init_error = 0;	//��ʼ�����󷵻أ������ʼ�����ڴ��󣬾ͽ��˱�����1
						//��Ҫ���ڳ�ʼ���ⲿ�豸������MPU6050��ʱ������ⲿ�豸�Ƿ���������
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	Time_Configuration();	//ϵͳʱ�����ʱ��ض�ʱ����ʼ��
	
	USART1_Config();		//USART1 ����ģʽΪ 115200 8-N-1���жϽ���
	USART2_Config();
	
	IIC_Init();	//���IIC��ʼ��
	
	init_error = mpu6050_init(5);	//MPU6050�ĳ�ʼ����Ҫ����systick���ȵĺ��棬��Ϊ���е�������ʱ����
									//�˺���������Ƕ�mpu6050�ڲ����ֵ�ͨ�˲�����Ƶ��������ã�����0����ر��ڲ���ͨ�˲���
	
	TIM_Input_Output_Configuration();	//������PWM�����ʼ��
	
	Attitude_Init();	//��ʼ����̬��ر���
	
	Balance_Init();		//���ȱ�����ʼ��
	
	//��ѭ����ʼ��������
	Loop_Init();		//ѭ�����Ʊ�����ʼ��
	SysTick_Init();		//�δ�ʱ����ʼ����1ms�ж�����
						//�����ʱ��������������ѭ�����У����ڳ�ʼ�������
						//Ҳ����Ӳ����ʼ����Ϻ�ʱ����ʼ���У������ƶ�����ʼ����
	
	return init_error;
}
