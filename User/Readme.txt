代码开发记录

2016.9.24
更新了卡尔曼相关的数据结构，使其同 卡尔曼滤波C代码分析.pdf 相对应
存在问题：Gyro_x的赋值代码被杠调，不知原因

mpu6050及卡尔曼滤波相关结构体：

1.
kalman.h：

KALMAN_STRUCT

2.
mpu6050_measure.h:

typedef struct 
{	
	xyz_f_t Acc_f;	//加速度
	xyz_f_t Gyro_f;	//角速度
	s16 Tempreature;
	
	xyz_f_t Acc_f_g;	//加速度(单位是g)
	xyz_f_t Gyro_f_g;	//角速度(单位是角度)
	
	xyz_s16_t Acc_temp[10];	//滑动滤波缓冲区
	xyz_s16_t Gyro_temp[10];	//滑动滤波缓冲区
	
}MPU6050_OUTPUT_STRUCT;

3.
bsp_mpu6050.h:

typedef struct 
{
	u8 mpu6050_buffer[14];
	
	xyz_s16_t Acc_I16;	//加速度输入原始值
	xyz_s16_t Gyro_I16;	//角速度输入原始值
	
	s16 Tempreature;
	
}MPU6050_STRUCT;

2016.9.25
加入 attitude computation.c 和 attitude computation.h
设计思路：
bsp_mpu6050提供数据读取
mpu6050_measure提供数据倍率变换和相关滤波
kalman提供姿态解算算法

在attitude computation.c中定义姿态结构体（卡尔曼结构体）作为姿态存储空间，调用卡尔曼相关函数进行解算
也就是说上面三个文件都是提供一些方法，在attitude computation.c将这些方法按照顺序组合，形成姿态解算

9.26
改造bsp_mpu6050和mpu6050_measure内函数结构，入参全部改为指针形式

mpu6050相关结构体移入attitude_computation.c
MPU6050_STRUCT mpu6050;
MPU6050_OUTPUT_STRUCT mpu6050_output;
