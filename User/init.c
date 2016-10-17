#include "stm32f10x.h"
#include "init.h"
#include "ioi2c.h"


u8 All_Init(void)
{
	int init_error = 0;	//初始化错误返回，如果初始化存在错误，就将此变量置1
						//主要用在初始化外部设备（比如MPU6050）时，检查外部设备是否正常工作
	
	USART1_Config();		//USART1 配置模式为 115200 8-N-1，中断接收
	NVIC_Configuration();
	
//硬件iic初始化函数，弃用
//	I2C_1_Init();	//初始化I2C1 这个IC2是连接在板上的eeprom上的，但只要不访问eeprom的地址，就可以照常使用
	
	IIC_Init();
	
	Loop_Init();		//循环控制变量初始化
	SysTick_Init();		//滴答定时器初始化，1ms中断周期
						//这个定时器用于驱动整个循环运行，放在初始化的最后
						//也就是硬件初始化完毕后定时器开始运行，进而推动任务开始调度
	
	init_error = mpu6050_init(5);	//MPU6050的初始化，要放在systick调度的后面，因为其中调用了延时函数
									//此函数的入参是对mpu6050内部数字低通滤波器的频带宽度设置，输入0代表关闭内部低通滤波器
	
	Attitude_Init();	//初始化姿态相关变量
	
	Balance_Init();		//自稳变量初始化
	
	return init_error;
}
