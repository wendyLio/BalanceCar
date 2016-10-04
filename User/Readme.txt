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

9.27
关于MPU6050模块：
MPU6050模块的AD0是内部通过一个电阻接GND。
悬空相当于低电平。接VCC能够得到高电平，不会造成VCC与GND直连。

10.3
工程更新为C8和ZE双编译版本。
对时间单位问题的解决：
由于使用的芯片是F103，理论上应该尽量减少float型变量的操作，所以在主函数内的时间参数均设定为
以u32型，毫秒为单位的变量
也就是说，1s在变量中被表示为 u32 dt = 1000;	//1000ms
对于kalman等需要float类型时间的函数，采用在函数内部手动转换的方式解决。

将attitude computation.c 和 attitude computation.h 改名为 attitude_computation.c 和 attitude_computation.h

参数类型选择问题：
u32 = uint32_t = typedef unsigned int
s32 = int32_t  = typedef   signed int

参考匿名飞控，决定把角度和角速度值用float型变量表示。在pid算法里面用p值调整放大倍数。
如果用s32类型表示，在乘p值的时候就有可能造成变量整体过大，有可能接近变量最大值。所以先用float，然后用p的倍率放大。


更新Accel_To_Angle函数

旧函数：
void Accel_To_Angle(xyz_f_t * Accel,xyz_f_t * Angle_t)
{
//	xyz_f_t Angle_t;	//用加速度计数据计算角度的估价值
	
	Angle_t->z = atan2(Accel->y,Accel->x)*180.0/PI;	//计算倾角，并转化为弧度制，取值范围0-2π，这个地方转化为弧度制好像很没用，还有点捣乱
	Angle_t->x = atan2(Accel->z,Accel->y)*180.0/PI;
	Angle_t->y = atan2(Accel->x,Accel->z)*180.0/PI;
	
//	return Angle_t;
}

新函数：
void Accel_To_Angle(xyz_f_t * Accel,xyz_f_t * Angle_t)
{
	Angle_t->z = atan2(Accel->y,Accel->x);	//计算倾角
	Angle_t->x = atan2(Accel->z,Accel->y);
	Angle_t->y = atan2(Accel->x,Accel->z);
}
