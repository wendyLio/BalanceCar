#include "attitude_computation.h"

MPU6050_STRUCT mpu6050;
MPU6050_OUTPUT_STRUCT mpu6050_output;	//mpu6050���ݶ�ȡ

KALMAN_STRUCT atti_x,atti_y;	//x��y����̬�Ŀ������˲��ṹ�壬���а������ٶȺͽǶ����

//mpu6050��ȡ������
xyz_f_t Gyro_mpu6050_out;	//mpu6050��ȡ�ľ����˲��Ľ��ٶ�
xyz_f_t Accel;				//mpu6050��ȡ�ļ��ٶ�
xyz_f_t Angle_estimate;		//�ü��ٶȹ��Ƴ��ĽǶ�

//�˲����
xyz_f_t Gyro;
xyz_f_t Angle;

void Attitude_Init(void)
{
	Gyro_mpu6050_out.x = 0.0;
	Gyro_mpu6050_out.y = 0.0;
	Gyro_mpu6050_out.z = 0.0;
	
	Accel.x = 0.0;
	Accel.y = 0.0;
	Accel.z = 0.0;
	
	Angle_estimate.x = 0.0;
	Angle_estimate.y = 0.0;
	Angle_estimate.z = 0.0;
	
	Kanman_Init(&atti_x);
	Kanman_Init(&atti_y);
}

void Attitude_sensor_Read(u32 T)
{
	//��IIC��ȡmpu6050���ݣ����ú����˲�
	mpu6050_Input_And_Measure(&mpu6050,&mpu6050_output,T);	
}

void Attitude_sensor_Update(u32 T)
{
//	//��IIC��ȡmpu6050���ݣ����ú����˲�
//	mpu6050_Input_And_Measure(&mpu6050,&mpu6050_output,T);	
	
	//��mpu6050_output��ֵ��Gyro
	//��Щ��ֵ����float�͵�
	Gyro_mpu6050_out.x = -mpu6050_output.Gyro_f_g.x;
	Gyro_mpu6050_out.y = -mpu6050_output.Gyro_f_g.y;
	Gyro_mpu6050_out.z = -mpu6050_output.Gyro_f_g.z;
	
	Accel.x = mpu6050_output.Acc_f_g.x;
	Accel.y = mpu6050_output.Acc_f_g.y;
	Accel.z = mpu6050_output.Acc_f_g.z;
	
	//ʹ�ü��ٶ�ֵ������Angle����ֵ
	//���Ե����������任������������У����������������ɷ����������������ת��ϵ
	Accel_To_Angle(&Accel,&Angle_estimate);	
	
	//�������˲���̬����
	Kanman_Filter(&atti_x,Gyro_mpu6050_out.x,Angle_estimate.x,T);
	Kanman_Filter(&atti_y,Gyro_mpu6050_out.y,Angle_estimate.y,T);
	
	//��������̬���
	//ֻ��x��y���ܹ����Ƶõ���z����û�д�����������޷�����
	Gyro.x = atti_x.Gyro_x;
	Gyro.y = atti_y.Gyro_x;
	
	Angle.x = atti_x.Angel;
	Angle.y = atti_y.Angel;
	
}
