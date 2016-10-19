#include "bsp_mpu6050.h"
#include "scheduler.h"
#include "time.h"
#include "ioi2c.h"


#define MPU6050_SLAVE_ADDRESS 0xD0	//AD0�ӵ�

//**********************************************�캽��mpu6050������ֲ**********************************************888

//����д����ת�Ӻ�����û��ʹ��SlaveAddress��Σ��˲����ڱ��ļ�ͷ���Ժ궨����ʽԤ��
//IIC��n�ֽ�����ת�Ӻ���
u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{
	return IICreadBytes(SlaveAddress, REG_Address, len,buf);
}

//I2C�ӿ�д��ת�Ӻ���
u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	IICwriteBytes(SlaveAddress, REG_Address, 1, &REG_data);
	return 0;
}



/***********************************************************************************************************
												����ʵ�ֺ���
***********************************************************************************************************/

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    MPU6050 ��˯��ģʽ����
				0 ��
				1 ��
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_SLAVE_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
* CLK_SEL | Clock Source
* --------+--------------------------------------
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source)
{
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_set_SMPLRT_DIV(uint16_t hz)
*��������:	    ���� MPU6050 �����������Ƶ��
*******************************************************************************/
void MPU6050_set_SMPLRT_DIV(uint16_t hz)
{
	IIC_Write_1Byte(MPU6050_SLAVE_ADDRESS, MPU6050_RA_SMPLRT_DIV,1000/hz - 1);
//	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleGyroRange(uint8_t range)
*��������:	    ����  MPU6050 ������ ���������
*******************************************************************************/
void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_GYRO_CONFIG,7, 3, 0x00);   //���Լ�
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_ACCEL_CONFIG,7, 3, 0x00);   //���Լ�
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setDLPF(uint8_t mode)
*��������:	    ����  MPU6050 ���ֵ�ͨ�˲���Ƶ�����
*******************************************************************************/
void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(MPU6050_SLAVE_ADDRESS, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_SLAVE_ADDRESS, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_SLAVE_ADDRESS, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

//*********************************************************************************************************
//							Ӧ�ú���
//*********************************************************************************************************

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬�����������ɿ��캽��mpu6050������
*******************************************************************************/
int MPU6050_Init(u16 lpf)
{ 
	u16 default_filter = 1;
	
	//ѡ��mpu6050�ڲ����ֵ�Ͳ�˲�������
	//�������ڲ���ͨ�˲��������ǲ����� 8MHz
	//  �����ڲ���ͨ�˲��������ǲ����� 1MHz
	//���ٶȼƲ�����1MHz
	switch(lpf)
	{
	case 5:
		default_filter = MPU6050_DLPF_BW_5;
		break;
	case 10:
		default_filter = MPU6050_DLPF_BW_10;
		break;
	case 20:
		default_filter = MPU6050_DLPF_BW_20;
		break;
	case 42:
		default_filter = MPU6050_DLPF_BW_42;
		break;
	case 98:
		default_filter = MPU6050_DLPF_BW_98;
		break;
	case 188:
		default_filter = MPU6050_DLPF_BW_188;
		break;
	case 256:
		default_filter = MPU6050_DLPF_BW_256;
		break;
	default:
		default_filter = MPU6050_DLPF_BW_42;
		break;
	}

	delay_ms(200);
	
	//�豸��λ
//	IIC_Write_1Byte(MPU6050_SLAVE_ADDRESS,MPU6050_RA_PWR_MGMT_1, 0x80);
	
	//����ʹ�õ�Delay()ֻ���ڳ�ʼ���׶�ʹ�ã����������ʹ������Delay()���Ῠ����������
	MPU6050_setSleepEnabled(0); //���빤��״̬
	Delay_ms(10);
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_ZGYRO);	//����ʱ��  0x6b   0x03
														//ʱ��Դѡ��MPU6050_CLOCK_INTERNAL��ʾ�ڲ�8M����
	Delay_ms(10);
	MPU6050_set_SMPLRT_DIV(1000);  //1000hz
	Delay_ms(10);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-2000��ÿ��
	Delay_ms(10);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_8);	//���ٶȶ�������� +-8G
	Delay_ms(10);
	MPU6050_setDLPF(default_filter);  //42hz
	Delay_ms(10);
	MPU6050_setI2CMasterModeEnabled(0);	 //����MPU6050 ����AUXI2C
	Delay_ms(10);
	MPU6050_setI2CBypassEnabled(1);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
	Delay_ms(10);
	
	return 0;
}



//��ȡMPU6050����Ĵ�����ֵ
void MPU6050_Read(MPU6050_STRUCT * mpu6050)
{
	IIC_Read_nByte(MPU6050_SLAVE_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050->mpu6050_buffer);
	
	/*ƴ��bufferԭʼ����*/
	mpu6050->Acc_I16.x = ((((int16_t)mpu6050->mpu6050_buffer[0]) << 8) | mpu6050->mpu6050_buffer[1]) ;
	mpu6050->Acc_I16.y = ((((int16_t)mpu6050->mpu6050_buffer[2]) << 8) | mpu6050->mpu6050_buffer[3]) ;
	mpu6050->Acc_I16.z = ((((int16_t)mpu6050->mpu6050_buffer[4]) << 8) | mpu6050->mpu6050_buffer[5]) ;
 
	mpu6050->Gyro_I16.x = ((((int16_t)mpu6050->mpu6050_buffer[ 8]) << 8) | mpu6050->mpu6050_buffer[ 9]) ;
	mpu6050->Gyro_I16.y = ((((int16_t)mpu6050->mpu6050_buffer[10]) << 8) | mpu6050->mpu6050_buffer[11]) ;
	mpu6050->Gyro_I16.z = ((((int16_t)mpu6050->mpu6050_buffer[12]) << 8) | mpu6050->mpu6050_buffer[13]) ;
	
	mpu6050->Tempreature = ((((int16_t)mpu6050->mpu6050_buffer[6]) << 8) | mpu6050->mpu6050_buffer[7]); //tempreature
	
}



