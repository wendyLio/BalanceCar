#include "mpu6050_measure.h"

int mpu6050_init(u16 lpf)
{
	return MPU6050_Init(lpf);
}

float TEM_LPF;	//�¶���ֵ��ͨ�˲��м����
float mpu6050_Temperature_Prepare(MPU6050_STRUCT * mpu6050,u32 T)
{
	//�¶������˲�����
	TEM_LPF += 2 *3.14f *T *(mpu6050->Tempreature - TEM_LPF);	//��һ�µ�ͨ�˲�
	return (TEM_LPF/340.0f + 36.5f);
}

void mpu6050_6axis_Prepare(MPU6050_STRUCT * original,MPU6050_OUTPUT_STRUCT * output)
{
	static int i = 0;
	int j;
	xyz_f_t Acc_sum = {0,0,0};
	xyz_f_t Gyro_sum = {0,0,0};
	
//*******************************�����˲�*********************************************************
	//ѭ����ֵ
	output->Acc_temp[i] = original->Acc_I16;
	output->Gyro_temp[i] = original->Gyro_I16;
	i++;
	if(i > 9) i = 0;
	
	//���
	for(j=0;j<10;j++)
	{
		Acc_sum.x += output->Acc_temp[j].x;
		Acc_sum.y += output->Acc_temp[j].y;
		Acc_sum.z += output->Acc_temp[j].z;
		
		Gyro_sum.x += output->Gyro_temp[j].x;
		Gyro_sum.y += output->Gyro_temp[j].y;
		Gyro_sum.z += output->Gyro_temp[j].z;
	}
	
	//ȡƽ��
	output->Acc_f.x = Acc_sum.x / 10.0;
	output->Acc_f.y = Acc_sum.y	/ 10.0;
	output->Acc_f.z = Acc_sum.z	/ 10.0;	
	output->Gyro_f.x = Gyro_sum.x / 10.0;
	output->Gyro_f.y = Gyro_sum.y / 10.0;
	output->Gyro_f.z = Gyro_sum.z / 10.0;
//******************************�����˲�����********************************************************
	
	//���е�λת��������2^16,�ٳ���������̣�
	//���Ի����ĵ�λӦ���� cm/s2 �� ��/s
	output->Acc_f_g.x  = output->Acc_f.x * To_CM_S2;	//���ٶȶ�������� +-8G
	output->Acc_f_g.y  = output->Acc_f.y * To_CM_S2;	//��ȡ����λ�� 16
	output->Acc_f_g.z  = output->Acc_f.z * To_CM_S2;
	output->Gyro_f_g.x = output->Gyro_f.x * To_Angle;	//������������� +-2000��ÿ��
	output->Gyro_f_g.y = output->Gyro_f.y * To_Angle;	//��ȡ����λ�� 16
	output->Gyro_f_g.z = output->Gyro_f.z * To_Angle;
}

void mpu6050_Input_And_Measure(MPU6050_STRUCT *mpu6050,MPU6050_OUTPUT_STRUCT *mpu6050_output,u32 T)
{
	MPU6050_Read(mpu6050);	//����MPU6050��������ȡ���ݣ��Զ�������ؽṹ��
	mpu6050_6axis_Prepare(mpu6050,mpu6050_output);
	mpu6050_output->Tempreature = mpu6050_Temperature_Prepare(mpu6050,T);
}


void Accel_To_Angle(xyz_f_t * Accel,xyz_f_t * Angle_t)
{
//	xyz_f_t Angle_t;	//�ü��ٶȼ����ݼ���ǶȵĹ���ֵ
	
	Angle_t->z = atan2(Accel->y,Accel->x);	//�������
	Angle_t->x = atan2(Accel->z,Accel->y);
	Angle_t->y = atan2(Accel->x,Accel->z);
	
//	return Angle_t;
}



