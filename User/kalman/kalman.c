#include "kalman.h"

void Kanman_Init(KALMAN_STRUCT * kalman)
{
	int i;
	
	//���
	(*kalman).Angel = 0.0;	//���Ź��ƵĽǶ�	�����սǶȽ��
	(*kalman).Gyro_x = 0.0;	//���Ź��ƽ��ٶ�
	
	//�̶�����
	(*kalman).Q_Angle = 0.001;		//{0.001,0.001,0.001};	//����������Э����	0.001�Ǿ���ֵ
	(*kalman).Q_Gyro = 0.003;		//{0.003,0.003,0.003};	//������Ư������Э����	��mpu6050�ľ���ֵ
	(*kalman).R_Angle = 0.5;		//{0.5,0.5,0.5};	//�Ǽ��ٶȼ�������Э����	
	
	(*kalman).C_0 = 1;		//{1,1,1};	//H�����һ���۲���� �ǳ���
	
	//�м���
	(*kalman).Q_Bias = 0;		//{0,0,0};		//������Ʈ��Ԥ��ֵ
	(*kalman).Angle_err = 0;	//{0,0,0};		//�����м�ֵ Angle �۲�ֵ-Ԥ��ֵ
	
	(*kalman).PCt_0 = 0;			//{0,0,0},	//�����м�ֵ
	(*kalman).PCt_1 = 0;			//{0,0,0},
	(*kalman).E     = 0;			//{0,0,0};
	(*kalman).t_0   = 0;			//{0,0,0},	//t:�����м����
	(*kalman).t_1   = 0;			//{0,0,0},
	
	(*kalman).K_0 = 0;			//{0,0,0},	//K:����������
	(*kalman).K_1 = 0;			//{0,0,0},
	
	for(i = 0;i < 4;i++)	//{0,0,0,0}	//����P������м����
	{
		(*kalman).Pdot[i] = 0;
	}
	
	(*kalman).PP[0][0] = 1;
	(*kalman).PP[0][1] = 0;
	(*kalman).PP[1][0] = 0;
	(*kalman).PP[1][1] = 1;
}


void Kanman_Filter(KALMAN_STRUCT * kalman,float Gyro,float Accel,u32 dt)	//Gyro�����ǵĲ���ֵ  |  Accel���ٶȼƵĽǶȼ�  |  dt��ʱ�俼����С�� �� ��С�ķֶȱ�ʾ
{
	float dt_f;
	
	//��dt�����λ��ms��u32�ͱ������ֵת��Ϊfloat�͵�����Ϊ��λ��ֵ
	dt_f = (float)dt;
	dt_f = dt_f / 1000;
	
	//x��ָ��ǰ��y��ָ���������ϵ   Ҫ�㸩����
	//��ô�����Ӧ����y��Ľ��ٶȣ�Gyro����y�����Ǽ��ٶȼƹ���ֵ
	//����ϵ������������
	
	
	//�ǶȲ���ģ�ͷ��� �Ƕȹ���ֵ=��һ�����нǶ�+�����ٶ�-��һ�ε�������Ʈ��*dt_f
	//��Ư����˵����Ϊÿ�ζ�����ͬ��Q_bias=Q_bias
	//���ƽǶ�
	(*kalman).Angel += (Gyro - (*kalman).Q_Bias) * dt_f;
	
	//�������ģ�͵ķ���
	(*kalman).Pdot[0] = (*kalman).Q_Angle - (*kalman).PP[0][1] - (*kalman).PP[1][0];
	(*kalman).Pdot[1] = -(*kalman).PP[1][1];
	(*kalman).Pdot[2] = -(*kalman).PP[1][1];
	(*kalman).Pdot[3] = (*kalman).Q_Gyro;
	
	(*kalman).PP[0][0] += (*kalman).Pdot[0] * dt_f;
	(*kalman).PP[0][1] += (*kalman).Pdot[1] * dt_f;
	(*kalman).PP[1][0] += (*kalman).Pdot[2] * dt_f;
	(*kalman).PP[1][1] += (*kalman).Pdot[3] * dt_f;
	
	//���㿨��������
	(*kalman).PCt_0 = (*kalman).C_0 * (*kalman).PP[0][0];	//����˷����м����
	(*kalman).PCt_1 = (*kalman).C_0 * (*kalman).PP[0][1];	//C_0=1
	(*kalman).E = (*kalman).R_Angle + (*kalman).C_0 * (*kalman).PCt_0;	//��ĸ
	(*kalman).K_0 = (*kalman).PCt_0 / (*kalman).E;	//���������棬������һ����Angle�ģ�һ����Q_bias��
	(*kalman).K_1 = (*kalman).PCt_1 / (*kalman).E;
	
	//�������ŽǶȡ�������Ʈ
	(*kalman).Angle_err = Accel - (*kalman).Angel;
	(*kalman).Angel += (*kalman).K_0 * (*kalman).Angle_err;	//�������ŵĽǶ�
	(*kalman).Q_Bias += (*kalman).K_1 * (*kalman).Angle_err;	//�������ŵ���Ʈ
	
	(*kalman).Gyro_x = Gyro -(*kalman).Q_Bias;	//��������Ž��ٶ�
	
	//���¹���ģ�͵ķ���
	(*kalman).t_0 = (*kalman).PCt_0;	//��������м�������൱��a
	(*kalman).t_1 = (*kalman).C_0 * (*kalman).PP[0][1];	//��������м�������൱��b
	
	(*kalman).PP[0][0] -= (*kalman).K_0 * (*kalman).t_0;
	(*kalman).PP[0][1] -= (*kalman).K_0 * (*kalman).t_1;
	(*kalman).PP[1][0] -= (*kalman).K_1 * (*kalman).t_0;
	(*kalman).PP[1][1] -= (*kalman).K_1 * (*kalman).t_1;
}
