#ifndef __TIME_H
#define	__TIME_H

#include "stm32f10x.h"

void delay_us(u32 us);
void delay_ms(u32 ms);
	
//void Delay_Ms(u32 myMs);		//whileʵ�ֵ���ʱ��������time.c����Ϊһ�ֿ�ѡ�����ʱ��ʽ�����ṩ�ⲿ�ӿ�
//void Delay_Us(u32 myUs);

//void Delay_us(u32 us);	//��timerʵ��

void Time_Configuration(void);
u32 readnowtime(void);

#endif /* __TMIE_H */
