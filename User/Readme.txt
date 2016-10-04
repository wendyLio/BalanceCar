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
