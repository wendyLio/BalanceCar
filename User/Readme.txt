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