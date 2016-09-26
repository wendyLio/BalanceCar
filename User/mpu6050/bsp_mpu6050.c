#include "bsp_mpu6050.h"
#include "bsp_i2c.h"
#include "scheduler.h"

#define MPU6050_SLAVE_ADDRESS 0xD0	//AD0½ÓµÍ
//#define MPU6050_SLAVE_ADDRESS 0xD2	//AD0½Ó¸ß

/**
  * @brief   Ğ´Ò»¸ö×Ö½Úµ½MPU6050ÖĞ
  *		@arg pBuffer:	1ByteĞÅÏ¢
  *		@arg WriteAddr:	Ğ´µØÖ· 
  */
void I2C_MPU6050_ByteWrite(u8 pBuffer, u8 WriteAddr)
{
	I2C_GenerateSTART(I2C1, ENABLE);	//Ğ´ÆğÊ¼ĞÅºÅ Send STRAT condition 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));	//¼ì²âI2CÍâÉèÊÂ¼ş EV5 ²¢Çå³ı¸ÃÊÂ¼ş 
																//³É¹¦·¢ËÍÆğÊ¼ĞÅºÅºó»á²úÉúĞÅºÅEV5
	I2C_Send7bitAddress(I2C1, MPU6050_SLAVE_ADDRESS, I2C_Direction_Transmitter);	//ÏòI2C×ÜÏßÉÏ·¢ËÍÒªĞ´ÈëĞÅÏ¢µÄÉè±¸µØÖ·
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//·¢ËÍÍêI2CÉè±¸Ñ°Ö·²¢µÃµ½Ó¦´ğºó ²úÉúEV6
	I2C_SendData(I2C1, WriteAddr);	//·¢ËÍMPU6050ÖĞÄ¿±ê¼Ä´æÆ÷µØÖ·
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	//¼ì²é·¢ËÍ³É¹¦ÊÂ¼ş
	I2C_SendData(I2C1, pBuffer); 	//·¢ËÍÒªĞ´ÈëµÄÊı¾İ£¨1×Ö½Ú£©
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	//¼ì²é·¢ËÍ³É¹¦ÊÂ¼ş
	I2C_GenerateSTOP(I2C1, ENABLE);	//·¢ËÍÍ£Ö¹ĞÅºÅ
}


/**
  * @brief   ´ÓMPU6050ÀïÃæ¶ÁÈ¡ n Byte Êı¾İ
  *		@arg pBuffer:´æ·Å´ÓMPU6050¶ÁÈ¡µÄÊı¾İµÄ»º³åÇøÖ¸Õë£¨Êı×éÊ×µØÖ·£©
  *		@arg WriteAddr:MPU6050ÖĞÄ¿±êÊı¾İ´æ·ÅµÄµØÖ·
  *     @arg NumByteToWrite:Òª´ÓMPU6050¶ÁÈ¡µÄ×Ö½ÚÊı
  *     
  *     ÆäÊµ°¡£¬Õâ¸öº¯ÊıÊÇ´ÓEEPROMÄÇÀï¸Ä¹ıÀ´µÄ£¬ÓµÓĞ¶ÁÈ¡n¸ö×Ö½ÚµÄÄÜÁ¦£¬µ«MPU6050µÄ¼Ä´æÆ÷ÆÕ±éÖ»ÓĞ1×Ö½Ú
  *     ×Ö½ÚÊıÄÇÀïÆ½Ê±¾ÍÊÇĞ´1£¬È»ºó»º³åÇøÖ¸Õë¾ÍÊÇÒ»¸ö±äÁ¿£¨int£©½øĞĞÈ¡µØÖ·
  */
void I2C_MPU6050_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
	//¶ÁÈ¡¹ı³ÌÓ¦¸ÃÊÇÕâÑùµÄ£ºÏÈÓÃĞ´µÄ·½Ê½£¬°ÑÒª¶ÁÈ¡µÄµØÖ·Ğ´½øÈ¥£¬È»ºóÔÙÓÃ¶ÁµÄ·½Ê½½ÓÊÕ¶ÔÃæµÄĞÅÏ¢¡£
	//ĞÅÏ¢½ÓÊÕÍê±ÏºóÖ÷¶¯Í£Ö¹Í¨Ñ¶
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); 	//¼ì²âSDAÊÇ·ñ¿ÕÏĞ£¨Ó¦¸ÃÊÇÎªÁËÊÊÓ¦¶àÖ÷»úÍ¨ĞÅ£©
													//Added by Najoua 27/08/2008    
	
	I2C_GenerateSTART(I2C1, ENABLE);	//·¢ËÍÆäÊµĞÅºÅ Send START condition */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));	//¼ì²éÆğÊ¼ĞÅºÅµÄ»ØÓ¦
	I2C_Send7bitAddress(I2C1, MPU6050_SLAVE_ADDRESS, I2C_Direction_Transmitter);	//·¢ËÍMPU6050µØÖ·
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1, ENABLE);	/* Clear EV6 by setting again the PE bit */
	I2C_SendData(I2C1, ReadAddr);	//·¢ËÍÒª¶ÁÈ¡µÄ¼Ä´æÆ÷µÄµØÖ·
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	//¼ì²éÊÇ·ñ·¢ËÍ³É¹¦
  
	I2C_GenerateSTART(I2C1, ENABLE);	//ÖØĞÂ·¢ËÍÆğÊ¼ĞÅºÅ
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));	//¼ì²éÆğÊ¼ĞÅºÅ·´À¡
	I2C_Send7bitAddress(I2C1, MPU6050_SLAVE_ADDRESS, I2C_Direction_Receiver);	//·¢ËÍMPU6050µØÖ·£¬Í¬Ê±±íÊ¾²ÉÓÃ¶ÁµÄ¹¤×÷·½Ê½
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));	//¼ì²é·´À¡

	while(NumByteToRead)  // While there is data to be read ÔÙÑ­»·Àï¶ÁÊı¾İ
	{
		//Ïàµ±ÓÚËµ£¬ÔÚ½ÓÊÜÍêµ¹ÊıµÚ¶ş¸ö×Ö½Úºó£¬Ö÷»úµÄI2C_ReceiveDataº¯Êı·¢³öÁË½ÓÊÜ³É¹¦Ó¦´ğ£¬È»ºó´Ó»ú·¢³öÁË×îºóÒ»¸ö×Ö½Ú£
		//È»ºóÖ÷»ú½ô½Ó×Å¹Ø±ÕÁË×îºóÒ»¸ö×Ö½ÚµÄ½ÓÊÕ³É¹¦·´À¡£¬Ö±½Ó·¢ËÍÁËÍ¨ĞÅ½áÊø±êÖ¾
		//Ö»ÄÜÈÏÎªÊÇÔÚ½ÓÊÕµ½µ¹ÊıµÚ¶ş×Ö½ÚµÄÓ¦´ğ³É¹¦ĞÅºÅºó£¬´Ó»úÁ¢¼´·¢ËÍÁË×îºóÒ»¸ö×Ö½Ú£¬ÕâÊ±Í£Ö¹ĞÅºÅ»¹Ã»·¢¹ıÀ´¡£
		//Ò²¾ÍÊÇËµÔÚÖ÷»ú½ÓÊÕµ½×îºóÒ»¸ö×Ö½ÚµÄÒ»Ë²¼ä£¨Ó²¼ş×Ô¶¯½ÓÊÕ£©£¬Ö÷»úÈí¼ş·¢ËÍÁËÍ£Ö¹ÃüÁî¡£
		//·¢ÍêÍ£Ö¹ÃüÁîºó£¬Ö÷»ú²ÅÈ¥¼ì²é×îºóÒ»¸ö×Ö½ÚÊÕµ½ÁËÊ²Ã´£¨ÊÕÃ»ÊÕµ½£©
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C1, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C1, ENABLE);
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			/* Read a byte from the EEPROM */
			*pBuffer = I2C_ReceiveData(I2C1);	//°ÑÊı¾İ´æ½ø´æ´¢Çø

			/* Point to the next location where the byte read will be saved */
			pBuffer++; //Ö¸ÕëÖ¸ÏòÏÂÒ»¸ö´æ´¢¿Õ¼ä£¨Êı×éÏÂÒ»Î»£©

			/* Decrement the read bytes counter */
			NumByteToRead--;        
		}   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);	//»Ö¸´I2CµÄ×Ô¶¯Ó¦´ğ
}


//**********************************************Áìº½Õßmpu6050Çı¶¯ÒÆÖ²**********************************************888
u8 mpu6050_ok;	// =1 Õı³£
				// =0 ´íÎó

//¶Á¡¢Ğ´Á½¸ö×ª½Óº¯Êı¶¼Ã»ÓĞÊ¹ÓÃSlaveAddressÈë²Î£¬´Ë²ÎÊıÔÚ±¾ÎÄ¼şÍ·²¿ÒÔºê¶¨ÒåĞÎÊ½Ô¤ÖÃ
//IIC¶Án×Ö½ÚÊı¾İ×ª½Óº¯Êı
u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{
	I2C_MPU6050_BufferRead(buf, REG_Address, len);
	return 0;
}

//I2C½Ó¿ÚĞ´Èë×ª½Óº¯Êı
u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	I2C_MPU6050_ByteWrite(REG_data, REG_Address);
	return 0;
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*¹¦¡¡¡¡ÄÜ:	  ¶Á ĞŞ¸Ä Ğ´ Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷Ò»¸ö×Ö½Ú ÖĞµÄ1¸öÎ»
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
reg	   ¼Ä´æÆ÷µØÖ·
bitNum  ÒªĞŞ¸ÄÄ¿±ê×Ö½ÚµÄbitNumÎ»
data  Îª0 Ê±£¬Ä¿±êÎ»½«±»Çå0 ·ñÔò½«±»ÖÃÎ»
·µ»Ø   ³É¹¦ Îª1 
Ê§°ÜÎª0
*******************************************************************************/ 
void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	IIC_Read_nByte(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	mpu6050_ok = !( IIC_Write_1Byte(dev, reg, b) );	//IIC_Write_1ByteÕı³£Çé¿ö·µ»Ø0£¬¾­¹ıÈ¥·´mpu6050_ok = 1
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*¹¦¡¡¡¡ÄÜ:	    ¶Á ĞŞ¸Ä Ğ´ Ö¸¶¨Éè±¸ Ö¸¶¨¼Ä´æÆ÷Ò»¸ö×Ö½Ú ÖĞµÄ¶à¸öÎ»
ÊäÈë	dev  Ä¿±êÉè±¸µØÖ·
reg	   ¼Ä´æÆ÷µØÖ·
bitStart  Ä¿±ê×Ö½ÚµÄÆğÊ¼Î»
length   Î»³¤¶È
data    ´æ·Å¸Ä±äÄ¿±ê×Ö½ÚÎ»µÄÖµ
·µ»Ø   ³É¹¦ Îª1 
Ê§°ÜÎª0
*******************************************************************************/ 
void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	
	u8 b,mask;
	IIC_Read_nByte(dev, reg, 1, &b);
	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b &= mask;
	b |= data;
	IIC_Write_1Byte(dev, reg, b);
}

/***********************************************************************************************************
												¹¦ÄÜÊµÏÖº¯Êı
***********************************************************************************************************/

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setSleepEnabled(uint8_t enabled)
*¹¦¡¡¡¡ÄÜ:	    MPU6050 µÄË¯ÃßÄ£Ê½¿ª¹Ø
				0 ¹Ø
				1 ¿ª
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setClockSource(uint8_t source)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ  MPU6050 µÄÊ±ÖÓÔ´
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
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_set_SMPLRT_DIV(uint16_t hz)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ MPU6050 µÄÍÓÂİÒÇÊä³öÆµÂÊ
*******************************************************************************/
void MPU6050_set_SMPLRT_DIV(uint16_t hz)
{
	IIC_Write_1Byte(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV,1000/hz - 1);
//	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setFullScaleGyroRange(uint8_t range)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ  MPU6050 ÍÓÂİÒÇ µÄ×î´óÁ¿³Ì
*******************************************************************************/
void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG,7, 3, 0x00);   //²»×Ô¼ì
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ  MPU6050 ¼ÓËÙ¶È¼ÆµÄ×î´óÁ¿³Ì
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG,7, 3, 0x00);   //²»×Ô¼ì
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setDLPF(uint8_t mode)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ  MPU6050 Êı×ÖµÍÍ¨ÂË²¨µÄÆµ´ø¿í¶È
*******************************************************************************/
void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(MPU6050_ADDRESS, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ MPU6050 ÊÇ·ñÎªAUX I2CÏßµÄÖ÷»ú
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*¹¦¡¡¡¡ÄÜ:	    ÉèÖÃ MPU6050 ÊÇ·ñÎªAUX I2CÏßµÄÖ÷»ú
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

/**************************ÊµÏÖº¯Êı********************************************
*º¯ÊıÔ­ĞÍ:		void MPU6050_initialize(void)
*¹¦¡¡¡¡ÄÜ:	    ³õÊ¼»¯ 	MPU6050 ÒÔ½øÈë¿ÉÓÃ×´Ì¬¡£¸Ä×ÔÄäÃû·É¿ØÁìº½Õßmpu6050Çı¶¯¡£
*******************************************************************************/
int MPU6050_Init(u16 lpf)
{ 
	u16 default_filter = 1;
	
	//Ñ¡Ôñmpu6050ÄÚ²¿Êı×ÖµÍÍ²ÂË²¨Æ÷´ø¿í
	//²»¿ªÆôÄÚ²¿µÍÍ¨ÂË²¨£¬ÍÓÂİÒÇ²ÉÑùÂÊ 8MHz
	//  ¿ªÆôÄÚ²¿µÍÍ¨ÂË²¨£¬ÍÓÂİÒÇ²ÉÑùÂÊ 1MHz
	//¼ÓËÙ¶È¼Æ²ÉÑùÂÊ1MHz
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

	//Éè±¸¸´Î»
//	IIC_Write_1Byte(MPU6050_SLAVE_ADDRESS,MPU6050_RA_PWR_MGMT_1, 0x80);
	
	//ÕâÀïÊ¹ÓÃµÄDelay()Ö»ÄÜÔÚ³õÊ¼»¯½×¶ÎÊ¹ÓÃ£¬ÈÎÎñµ÷¶ÈÖĞÊ¹ÓÃÕâÖÖDelay()£¬»á¿¨ËÀÕû¸öµ÷¶È
	MPU6050_setSleepEnabled(0); //½øÈë¹¤×÷×´Ì¬
	Delay_ms(10);
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_ZGYRO);	//ÉèÖÃÊ±ÖÓ  0x6b   0x03
														//Ê±ÖÓÔ´Ñ¡Ôñ£¬MPU6050_CLOCK_INTERNAL±íÊ¾ÄÚ²¿8M¾§Õñ
	Delay_ms(10);
	MPU6050_set_SMPLRT_DIV(1000);  //1000hz
	Delay_ms(10);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//ÍÓÂİÒÇ×î´óÁ¿³Ì +-2000¶ÈÃ¿Ãë
	Delay_ms(10);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_8);	//¼ÓËÙ¶È¶È×î´óÁ¿³Ì +-8G
	Delay_ms(10);
	MPU6050_setDLPF(default_filter);  //42hz
	Delay_ms(10);
	MPU6050_setI2CMasterModeEnabled(0);	 //²»ÈÃMPU6050 ¿ØÖÆAUXI2C
	Delay_ms(10);
	MPU6050_setI2CBypassEnabled(1);	 //Ö÷¿ØÖÆÆ÷µÄI2CÓë	MPU6050µÄAUXI2C	Ö±Í¨¡£¿ØÖÆÆ÷¿ÉÒÔÖ±½Ó·ÃÎÊHMC5883L
	Delay_ms(10);
	
	return (mpu6050_ok == 0);
}



//¶ÁÈ¡MPU6050Êä³ö¼Ä´æÆ÷ÊıÖµ
void MPU6050_Read(MPU6050_STRUCT * mpu6050)
{
	IIC_Read_nByte(MPU6050_SLAVE_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050->mpu6050_buffer);
	
	/*Æ´½ÓbufferÔ­Ê¼Êı¾İ*/
	mpu6050->Acc_I16.x = ((((int16_t)mpu6050->mpu6050_buffer[0]) << 8) | mpu6050->mpu6050_buffer[1]) ;
	mpu6050->Acc_I16.y = ((((int16_t)mpu6050->mpu6050_buffer[2]) << 8) | mpu6050->mpu6050_buffer[3]) ;
	mpu6050->Acc_I16.z = ((((int16_t)mpu6050->mpu6050_buffer[4]) << 8) | mpu6050->mpu6050_buffer[5]) ;
 
	mpu6050->Gyro_I16.x = ((((int16_t)mpu6050->mpu6050_buffer[ 8]) << 8) | mpu6050->mpu6050_buffer[ 9]) ;
	mpu6050->Gyro_I16.y = ((((int16_t)mpu6050->mpu6050_buffer[10]) << 8) | mpu6050->mpu6050_buffer[11]) ;
	mpu6050->Gyro_I16.z = ((((int16_t)mpu6050->mpu6050_buffer[12]) << 8) | mpu6050->mpu6050_buffer[13]) ;
	
	mpu6050->Tempreature = ((((int16_t)mpu6050->mpu6050_buffer[6]) << 8) | mpu6050->mpu6050_buffer[7]); //tempreature
	
}


////Ğ´¸ñÊ½×ª»»º¯Êı
//void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
//{
//	I2C_MPU6050_ByteWrite(reg_dat,reg_add);
//}

////¶Á¸ñÊ½×ª»»º¯Êı
//void MPU6050_ReadData(u8 reg_add,unsigned char* Read,u8 num)
//{
//	I2C_MPU6050_BufferRead(Read,reg_add,num);
//}

////´Ë¶Îº¯ÊıÊÇ¿½±´À´µÄ£¬Ö»ÄÜ±£Ö¤´«¸ĞÆ÷¹¤×÷£¬²»ÄÜ±£Ö¤Êı¾İ×¼È·ĞÔ
////Æô¶¯Ê±Ã»ÓĞ×Ô¼ì£¬È±·¦Ğ£×¼
//void MPU6050_Init(void)
//{
//	//ÔÚ³õÊ¼»¯Ö®Ç°ÒªÑÓÊ±Ò»¶ÎÊ±¼ä£¬ÈôÃ»ÓĞÑÓÊ±£¬Ôò¶ÏµçºóÔÙÉÏµçÊı¾İ¿ÉÄÜ»á³ö´í
//	Delay_ms(500);
//		
//	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //½â³ıĞİÃß×´Ì¬ ¹Ø±ÕÑ­»·Ä£Ê½ ÆôÓÃÎÂ¶È´«¸ĞÆ÷ Ê¹ÓÃÄÚ²¿8M¾§Õñ
//	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //ÍÓÂİÒÇ²ÉÑùÂÊ ÉèÖÃÍÓÂİÒÇÊä³ö·ÖÆµ£¬´Ó¶øµÃµ½²ÉÑùÂÊ 
//														//²ÉÑùÂÊ = ÍÓÂİÒÇÊä³öÆµÂÊ / (1 + SMPLRT_DIV)
//	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);			//ÅäÖÃ5Hz´ø¿íµÄÊı×ÖµÍÍ¨ÂË²¨
//	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //ÅäÖÃ¼ÓËÙ¶È´«¸ĞÆ÷¹¤×÷ÔÚ2GÄ£Ê½
//	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //ÍÓÂİÒÇ×Ô¼ì¼°²âÁ¿·¶Î§£¬µäĞÍÖµ£º0x18(²»×Ô¼ì£¬2000deg/s)
//}

////¶ÁÆ÷¼şµØÖ·
//unsigned char MPU6050ReadID(void)
//{
//	unsigned char Re = 0;
//    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //¶ÁÆ÷¼şµØÖ·
////     printf("%d\r\n",Re);
//	return Re;
//}

////ÕâÀïµÄx£¬y£¬zÊÇÏÈĞ´ÉÏÈ¥µÄ£¬Ã»ÓĞºËÊµ¹ı£¬Ê¹ÓÃÇ°Ò»¶¨ÒªºËÊµ£¡£¡£¡
//void MPU6050ReadAcc(mpu6050_3D *accData)
//{
//    u8 buf[6];
//    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
//	accData->x = (buf[0] << 8) | buf[1];
//	accData->y = (buf[2] << 8) | buf[3];
//	accData->z = (buf[4] << 8) | buf[5];
//}
//void MPU6050ReadGyro(mpu6050_3D *gyroData)
//{
//    u8 buf[6];
//    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
//	gyroData->x = (buf[0] << 8) | buf[1];
//	gyroData->y = (buf[2] << 8) | buf[3];
//	gyroData->z = (buf[4] << 8) | buf[5];
//}

//void MPU6050ReadTemp(short *tempData)	//short 16Î»
//{
//	u8 buf[2];
//    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //¶ÁÈ¡ÎÂ¶ÈÖµ
//	
//    *tempData = (buf[0] << 8) | buf[1];		//¼òµ¥µÄ°Ñ2¸ö8Î»Æ´³É16Î»
//}

//void MPU6050_ReturnTemp(short*Temperature)	
//{
//	short temp3;
//	u8 buf[2];
//	
//	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //¶ÁÈ¡ÎÂ¶ÈÖµ
//	temp3= (buf[0] << 8) | buf[1];
//	
//	*Temperature=(((double) (temp3 + 13200)) / 280)-13;	//¶ÁÈ¡ÎÂ¶Èºó½øĞĞÁËÒ»¸ö¹«Ê½»»Ëã£¬ÔİÊ±²»È·¶¨»»Ëã½á¹ûµÄÎïÀíÒâÒå
//}


//void MPU6050ReadAcc(short *accData)
//{
//    u8 buf[6];
//    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
//    accData[0] = (buf[0] << 8) | buf[1];
//    accData[1] = (buf[2] << 8) | buf[3];
//    accData[2] = (buf[4] << 8) | buf[5];
//}
//void MPU6050ReadGyro(short *gyroData)
//{
//    u8 buf[6];
//    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
//    gyroData[0] = (buf[0] << 8) | buf[1];
//    gyroData[1] = (buf[2] << 8) | buf[3];
//    gyroData[2] = (buf[4] << 8) | buf[5];
//}
