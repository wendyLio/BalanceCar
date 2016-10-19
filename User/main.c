#include "stm32f10x.h"
#include "init.h"
#include "scheduler.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int init_error;

int main(void)
{	
	init_error = All_Init();
	
	if(init_error)
		printf("Ӳ����ʼ����������\n");	
	else
		printf("Ӳ����ʼ������\n");	

	while (1)
	{	 
		Duty_Loop();
	}

}
/*********************************************END OF FILE**********************/
