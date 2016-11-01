���뿪����¼

2016.9.24
�����˿�������ص����ݽṹ��ʹ��ͬ �������˲�C�������.pdf ���Ӧ
�������⣺Gyro_x�ĸ�ֵ���뱻�ܵ�����֪ԭ��

mpu6050���������˲���ؽṹ�壺

1.
kalman.h��

KALMAN_STRUCT

2.
mpu6050_measure.h:

typedef struct 
{	
	xyz_f_t Acc_f;	//���ٶ�
	xyz_f_t Gyro_f;	//���ٶ�
	s16 Tempreature;
	
	xyz_f_t Acc_f_g;	//���ٶ�(��λ��g)
	xyz_f_t Gyro_f_g;	//���ٶ�(��λ�ǽǶ�)
	
	xyz_s16_t Acc_temp[10];	//�����˲�������
	xyz_s16_t Gyro_temp[10];	//�����˲�������
	
}MPU6050_OUTPUT_STRUCT;

3.
bsp_mpu6050.h:

typedef struct 
{
	u8 mpu6050_buffer[14];
	
	xyz_s16_t Acc_I16;	//���ٶ�����ԭʼֵ
	xyz_s16_t Gyro_I16;	//���ٶ�����ԭʼֵ
	
	s16 Tempreature;
	
}MPU6050_STRUCT;

2016.9.25
���� attitude computation.c �� attitude computation.h
���˼·��
bsp_mpu6050�ṩ���ݶ�ȡ
mpu6050_measure�ṩ���ݱ��ʱ任������˲�
kalman�ṩ��̬�����㷨

��attitude computation.c�ж�����̬�ṹ�壨�������ṹ�壩��Ϊ��̬�洢�ռ䣬���ÿ�������غ������н���
Ҳ����˵���������ļ������ṩһЩ��������attitude computation.c����Щ��������˳����ϣ��γ���̬����

9.26
����bsp_mpu6050��mpu6050_measure�ں����ṹ�����ȫ����Ϊָ����ʽ

mpu6050��ؽṹ������attitude_computation.c
MPU6050_STRUCT mpu6050;
MPU6050_OUTPUT_STRUCT mpu6050_output;

9.27
����MPU6050ģ�飺
MPU6050ģ���AD0���ڲ�ͨ��һ�������GND��
�����൱�ڵ͵�ƽ����VCC�ܹ��õ��ߵ�ƽ���������VCC��GNDֱ����

10.3
���̸���ΪC8��ZE˫����汾��
��ʱ�䵥λ����Ľ����
����ʹ�õ�оƬ��F103��������Ӧ�þ�������float�ͱ����Ĳ������������������ڵ�ʱ��������趨Ϊ
��u32�ͣ�����Ϊ��λ�ı���
Ҳ����˵��1s�ڱ����б���ʾΪ u32 dt = 1000;	//1000ms
����kalman����Ҫfloat����ʱ��ĺ����������ں����ڲ��ֶ�ת���ķ�ʽ�����

��attitude computation.c �� attitude computation.h ����Ϊ attitude_computation.c �� attitude_computation.h

��������ѡ�����⣺
u32 = uint32_t = typedef unsigned int
s32 = int32_t  = typedef   signed int

�ο������ɿأ������ѽǶȺͽ��ٶ�ֵ��float�ͱ�����ʾ����pid�㷨������pֵ�����Ŵ�����
�����s32���ͱ�ʾ���ڳ�pֵ��ʱ����п�����ɱ�����������п��ܽӽ��������ֵ����������float��Ȼ����p�ı��ʷŴ�


����Accel_To_Angle����

�ɺ�����
void Accel_To_Angle(xyz_f_t * Accel,xyz_f_t * Angle_t)
{
//	xyz_f_t Angle_t;	//�ü��ٶȼ����ݼ���ǶȵĹ���ֵ
	
	Angle_t->z = atan2(Accel->y,Accel->x)*180.0/PI;	//������ǣ���ת��Ϊ�����ƣ�ȡֵ��Χ0-2�У�����ط�ת��Ϊ�����ƺ����û�ã����е㵷��
	Angle_t->x = atan2(Accel->z,Accel->y)*180.0/PI;
	Angle_t->y = atan2(Accel->x,Accel->z)*180.0/PI;
	
//	return Angle_t;
}

�º�����
void Accel_To_Angle(xyz_f_t * Accel,xyz_f_t * Angle_t)
{
	Angle_t->z = atan2(Accel->y,Accel->x);	//�������
	Angle_t->x = atan2(Accel->z,Accel->y);
	Angle_t->y = atan2(Accel->x,Accel->z);
}

10.4
������bsp_pwm_out�е��ٶ����뺯����
������schedule.c��

����data_transfer.c��data_transfer.h��
��usart1�Ľ����ж����������жϴ�������
����׼���ڽ����ж���������ʶ��֧�ִ��ڵ���pid��

�������ˣ�uint8_t�е�t��typedef����˼��

10.5
����movement_control�ļ����γɼ� ������� �� �ٶȲɼ� Ϊһ���Ӳ�������

���Э�鼰���������
/*
	���ȣ�		7 Byte

	Byte1	��ͷ		0xAA
	Byte2	ʶ���		1~255
	Byte3	����1
	Byte4	����2
	Byte5	����3
	Byte6	����4
	Byte7	��β		0xBB
	
*/

2016.10.14
��scheduler.c�����delay_ms����߼����иĶ�
	if(delay_ms_counter != 0)
��Ϊ
	if(delay_ms_counter >= 0)
	
ɾ����	
//��仰Ӧ�ò��������ã�������ǿ³���԰�
//���delay_ms_counter<0�ˣ�������ʱ�߼��ͳ���������
if(delay_ms_counter < 0)
	delay_ms_counter = 0;
	
bsp_i2c.c��I2C_1_Init()�����

//**************************************************************************
//�¼��벿�֣������ͷ�I2C����
//**************************************************************************
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;//IIC??PB6--SCL,PB7--SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	for(i=0;i<10;i++)
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	delay_us(100);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	delay_us(100);
	}
	
//**************************************************************************
//����
//**************************************************************************

��

//�ٶ��Ŀ������һƬ�ĵ�˵Ҫʹ��Ӧ����
I2C_AcknowledgeConfig(I2C1, ENABLE);//ʹ��Ӧ����

���ּ���Ĵ��붼û���ã����񻹴�����������

��IICwriteBit�������һ��100us��delay�ͺ���
void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	Delay_Us(100);		//������100us�ӳ�
	IIC_Read_nByte(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	mpu6050_ok = !( IIC_Write_1Byte(dev, reg, b) );	//IIC_Write_1Byte�����������0������ȥ��mpu6050_ok = 1
}


10.17

�������I2C

Accel_To_Angle�ڸ��ýǶ���

����mpu6050���ϵ��ֱ�ӳ�ʼ������������0,�ڳ�ʼ��ǰ��200ms��ʱ�������

���Ӵ���2������������2�����жϣ���û��ת��printf������printf��������usart1��

��All_Init������ͳһ�����ж����ȼ����鷽ʽ

����All_Init�����ṹ

������systickΪ��ʱ����Delay_ms()��������ʱ������whileѭ����ʽʵ�ֵ�Delay_Ms()��Delay_Us()
�����𲽿�ʼʹ�û���timer����ʱ����

����ת����Ʋ��ֻ�û��д�꣬ ����ѡ�û�û�о������


����������
1.ȷ��Timerʹ�����
2.ȷ��GPIOʹ�����

��ʱ����
C8��3��ͨ�ö�ʱ����1���߼���ʱ��
ZE��4��ͨ�ö�ʱ����2���߼���ʱ����1��������ʱ��

GPIO��
C8��PA0-15
	PB0-15
	PC13-15
	
	USART1 TX PA9
		   RX PA10
	
	USART2 TX PA2
		   Rx PA3
	
	USART3 TX PB10
		   RX PB11
		   
	Soft_IIC PB8
		     PB9
	
	�����
	Timer3 CH3 PB0 
		   CH4 PB1
		   
	���룺
	Timer1 CH1 PA8
	Timer3 CH1 PA6
	
	��ʱ��
	Timer4
	
	ϵͳʱ�����
	Timer2
	
	������ƣ�
	PA1
	PA2
	PA3
	PA4
	
10.20
���������˵�����ƺ������������������Ϊs32�ͣ�����pwm���ֻ֧��u16�����⣬�˴����ܵ�����ֵ���
PWM��������������õ�������ֵΪ1000����֪��1000�ǲ���������ֵ

Get_Speed��ȱ�ٺ�����������ĵ�λ����

11.1
��IO�˿ڽ��йٷ�����ƥ��

��ɵ�����Ƶ�4����������Ϊ
AIN2:GPIOB 15
AIN1:GPIOB 14
BIN1:GPIOB 13
BIN2:GPIOB 12

�� movement_control��Timer_In_Out ��� bsp_motor��bsp_encoder��movement_control��
	
		   



