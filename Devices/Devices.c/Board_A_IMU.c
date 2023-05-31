/**
 * @file Board_A_IMU.c
 * @author Leo Liu
 * @brief communicate and obtain data from board A IMU_Calculated_Data
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Board_A_IMU.h"

#define BOARD_A_IMU_SPI hspi5 
#define SPI_NSS_LOW HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define SPI_NSS_HIGH HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)

Board_A_IMU_t Board_A_IMU;
FusionAhrs Board_A_IMU_AHRS;

void Board_A_IMU_Init(void);
void Board_A_IMU_Calibrate(Board_A_IMU_t *Board_A_IMU);
void Board_A_IMU_Read_Data(Board_A_IMU_t *Board_A_IMU);
void Board_A_IMU_Calc_Angle(Board_A_IMU_t *Board_A_IMU);
void Board_A_IMU_Reset(Board_A_IMU_t *Board_A_IMU);

Board_A_IMU_Func_t Board_A_IMU_Func = Board_A_IMU_Func_GroundInit;
#undef Board_A_IMU_Func_GroundInit

static uint8_t tx, rx;
static uint8_t tx_buff[14] = {0xff};

static HAL_StatusTypeDef Board_A_IMU_Read_Bytes(uint8_t const regAddr, uint8_t *pData, uint8_t len);
static uint8_t Board_A_IMU_Read_Byte(uint8_t const reg);
static uint8_t Board_A_IMU_Write_Byte(uint8_t const reg, uint8_t const data);
static uint8_t IST8310_Init(void);;
static void IST8310_Write_Byte(uint8_t addr, uint8_t data);
static void Board_A_IMU_I2C_Config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
static uint8_t IST8310_Read_Byte(uint8_t addr);


void Board_A_IMU_Init(void)
{
	uint8_t i = 0;
	uint8_t MPU6500_Init_Data[10][2] = {
			{MPU6500_PWR_MGMT_1, 0x80},     /* Reset Device */
			{MPU6500_PWR_MGMT_1, 0x03},     /* Clock Source - Gyro-Z */
			{MPU6500_PWR_MGMT_2, 0x00},     /* Enable Acc & Gyro */
			{MPU6500_CONFIG, 0x04},         /* LPF 41Hz */
			{MPU6500_GYRO_CONFIG, 0x18},    /* +-2000dps */
			{MPU6500_ACCEL_CONFIG, 0x10},   /* +-8G */
			{MPU6500_ACCEL_CONFIG_2, 0x02}, /* Enable LowPassFilter  Set Acc LPF */
			{MPU6500_USER_CTRL, 0x20},
	}; /* Enable AUX */
	for (i = 0; i < 10; i++)
	{
			Board_A_IMU_Write_Byte(MPU6500_Init_Data[i][0], MPU6500_Init_Data[i][1]);
			HAL_Delay(1);
	}
	IST8310_Init();
}

static HAL_StatusTypeDef Board_A_IMU_Read_Bytes(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
    HAL_StatusTypeDef Info_Update_Flag = HAL_ERROR;
    SPI_NSS_LOW;
    tx = regAddr | 0x80;
    tx_buff[0] = tx;
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    Info_Update_Flag = HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, tx_buff, pData, len, 55);
    SPI_NSS_HIGH;
    return Info_Update_Flag;
}

static uint8_t Board_A_IMU_Read_Byte(uint8_t const reg)
{
    SPI_NSS_LOW;
    tx = reg | 0x80;
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    SPI_NSS_HIGH;
    return rx;
}

static uint8_t Board_A_IMU_Write_Byte(uint8_t const reg, uint8_t const data)
{
    SPI_NSS_LOW;
    tx = reg & 0x7F;
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    tx = data;
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    SPI_NSS_HIGH;
    return 0;
}

static uint8_t IST8310_Init(void)
{
    /* enable iic master mode */
    Board_A_IMU_Write_Byte(MPU6500_USER_CTRL, 0x30);
    HAL_Delay(10);
    /* enable iic 400khz */
    Board_A_IMU_Write_Byte(MPU6500_I2C_MST_CTRL, 0x0d);
    HAL_Delay(10);

    /* turn on slave 1 for ist write and slave 4 to ist read */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_ADDR, IST8310_ADDRESS);
    HAL_Delay(10);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS);
    HAL_Delay(10);

    /* IST8310_R_CONFB 0x01 = device rst */
    IST8310_Write_Byte(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);
    if (IST8310_DEVICE_ID_A != IST8310_Read_Byte(IST8310_WHO_AM_I))
        return 1;

    /* soft reset */
    IST8310_Write_Byte(IST8310_R_CONFB, 0x01);
    HAL_Delay(10);

    /* config as ready mode to access register */
    IST8310_Write_Byte(IST8310_R_CONFA, 0x00);
    if (IST8310_Read_Byte(IST8310_R_CONFA) != 0x00)
        return 2;
    HAL_Delay(10);

    /* normal state, no int */
    IST8310_Write_Byte(IST8310_R_CONFB, 0x00);
    if (IST8310_Read_Byte(IST8310_R_CONFB) != 0x00)
        return 3;
    HAL_Delay(10);

    /* config low noise mode, x,y,z axis 16 time 1 avg */
    IST8310_Write_Byte(IST8310_AVGCNTL, 0x24); //100100
    if (IST8310_Read_Byte(IST8310_AVGCNTL) != 0x24)
        return 4;
    HAL_Delay(10);

    /* Set/Reset pulse duration setup,normal mode */
    IST8310_Write_Byte(IST8310_PDCNTL, 0xc0);
    if (IST8310_Read_Byte(IST8310_PDCNTL) != 0xc0)
        return 5;
    HAL_Delay(10);

    /* turn off slave1 & slave 4 */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_CTRL, 0x00);
    HAL_Delay(10);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_CTRL, 0x00);
    HAL_Delay(10);

    /* configure and turn on slave 0 */
    Board_A_IMU_I2C_Config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
    HAL_Delay(100);
    return 0;
}

static void IST8310_Write_Byte(uint8_t addr, uint8_t data)
{
    /* turn off slave 1 at first */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_CTRL, 0x00);
    HAL_Delay(2);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_REG, addr);
    HAL_Delay(2);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_DO, data);
    HAL_Delay(2);
    /* turn on slave 1 with one byte transmitting */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
    /* wait longer to ensure the data is transmitted from slave 1 */
    HAL_Delay(10);
}

static uint8_t IST8310_Read_Byte(uint8_t addr)
{
    uint8_t retval;
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_REG, addr);
    HAL_Delay(10);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_CTRL, 0x80);
    HAL_Delay(10);
    retval = Board_A_IMU_Read_Byte(MPU6500_I2C_SLV4_DI);
    /* turn off slave4 after read */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_CTRL, 0x00);
    HAL_Delay(10);
    return retval;
}   

static void Board_A_IMU_I2C_Config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num)
{
    /* 
	   * configure the device address of the IST8310 
     * use slave1, auto transmit single measure mode 
	   */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_ADDR, device_address);
    HAL_Delay(2);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_REG, IST8310_R_CONFA);
    HAL_Delay(2);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_DO, IST8310_ODR_MODE);
    HAL_Delay(2);

    /* use slave0,auto read data */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV0_ADDR, 0x80 | device_address);
    HAL_Delay(2);
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV0_REG, reg_base_addr);
    HAL_Delay(2);

    /* every eight mpu6500 internal samples one i2c master read */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV4_CTRL, 0x03);
    HAL_Delay(2);
    /* enable slave 0 and 1 access delay */
    Board_A_IMU_Write_Byte(MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
    HAL_Delay(2);
    /* enable slave 1 auto transmit */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
    /* Wait 6ms (minimum waiting time for 16 times internal average setup) */
    HAL_Delay(6);
    /* enable slave 0 with data_num bytes reading */
    Board_A_IMU_Write_Byte(MPU6500_I2C_SLV0_CTRL, 0x80 | data_num);
    HAL_Delay(2);
}

//Calibrate board A IMU to reduce zero deviation
void Board_A_IMU_Calibrate(Board_A_IMU_t *Board_A_IMU)
{
	if(Board_A_IMU->Calibrated_Flag == 0)
	{
		float Buffer[6];
		
		for(int i = 0; i < CALIBRATION_SAMPLE; i++)
		{
			Board_A_IMU_Read_Data(Board_A_IMU);
			Buffer[0] += Board_A_IMU->Calc_Data.Ax;
			Buffer[1] += Board_A_IMU->Calc_Data.Ay;
			Buffer[2] += Board_A_IMU->Calc_Data.Az;
			Buffer[3] += Board_A_IMU->Calc_Data.Gx;
			Buffer[4] += Board_A_IMU->Calc_Data.Gy;
			Buffer[5] += Board_A_IMU->Calc_Data.Gz;
			HAL_Delay(1);
		}

		Board_A_IMU->Offset.Ax = Buffer[0] / CALIBRATION_SAMPLE;
		Board_A_IMU->Offset.Ay = Buffer[1] / CALIBRATION_SAMPLE;
		Board_A_IMU->Offset.Az = Buffer[2] / CALIBRATION_SAMPLE - 1.0f;
		Board_A_IMU->Offset.Gx = Buffer[3] / CALIBRATION_SAMPLE;
		Board_A_IMU->Offset.Gy = Buffer[4] / CALIBRATION_SAMPLE;
		Board_A_IMU->Offset.Gz = Buffer[5] / CALIBRATION_SAMPLE;
		
		Board_A_IMU->Calibrated_Flag = 1;
	}
	else
		;
}


void Board_A_IMU_Read_Data(Board_A_IMU_t *Board_A_IMU)
{
	uint8_t Buffer[14];
	uint8_t Buffer_Mag[6];
	
	//Calculate sampling period
	Board_A_IMU->Sample.Now_Time = HAL_GetTick() / 1000.0f;
	Board_A_IMU->Sample.Period = Board_A_IMU->Sample.Now_Time - Board_A_IMU->Sample.Prev_Time;
	Board_A_IMU->Sample.Prev_Time = Board_A_IMU->Sample.Now_Time;
	
	Board_A_IMU->Offline_Flag = Board_A_IMU_Read_Bytes(MPU6500_ACCEL_XOUT_H, Buffer, 14);
	Board_A_IMU_Read_Bytes(MPU6500_EXT_SENS_DATA_00, Buffer_Mag, 6);

	//Obtain data from buffer
	Board_A_IMU->Raw_Data.Ax = ((int16_t)Buffer[0] << 8) | Buffer[1];
	Board_A_IMU->Raw_Data.Ay = ((int16_t)Buffer[2] << 8) | Buffer[3];
	Board_A_IMU->Raw_Data.Az = ((int16_t)Buffer[4] << 8) | Buffer[5];
	Board_A_IMU->Raw_Data.Temperature = ((int16_t)Buffer[6] << 8) | Buffer[7];
	Board_A_IMU->Raw_Data.Gx = ((int16_t)Buffer[8] << 8) | Buffer[9];
	Board_A_IMU->Raw_Data.Gy = ((int16_t)Buffer[10] << 8) | Buffer[11];
	Board_A_IMU->Raw_Data.Gz = ((int16_t)Buffer[12] << 8) | Buffer[13];	
	memcpy(&Board_A_IMU->Raw_Data.Mx, Buffer_Mag, 6);
	
	Board_A_IMU->Calc_Data.Ax = Board_A_IMU->Raw_Data.Ax / 4096.0f;
	Board_A_IMU->Calc_Data.Ay = Board_A_IMU->Raw_Data.Ay / 4096.0f;
	Board_A_IMU->Calc_Data.Az = Board_A_IMU->Raw_Data.Az / 4096.0f;
	Board_A_IMU->Calc_Data.Gx = Board_A_IMU->Raw_Data.Gx / 16.384f;
	Board_A_IMU->Calc_Data.Gy = Board_A_IMU->Raw_Data.Gy / 16.384f;
	Board_A_IMU->Calc_Data.Gz = Board_A_IMU->Raw_Data.Gz / 16.384f;
	Board_A_IMU->Calc_Data.Mx = Board_A_IMU->Raw_Data.Mx;
	Board_A_IMU->Calc_Data.My = Board_A_IMU->Raw_Data.My;
	Board_A_IMU->Calc_Data.Mz = Board_A_IMU->Raw_Data.Mz;
	Board_A_IMU->Calc_Data.Temperature = Board_A_IMU->Raw_Data.Temperature / 333.87f + 21.0f;
	
	if(Board_A_IMU->Calibrated_Flag == 1) //If calibration is done, offset should be deducted
	{
		Board_A_IMU->Calc_Data.Ax -= Board_A_IMU->Offset.Ax;
		Board_A_IMU->Calc_Data.Ay -= Board_A_IMU->Offset.Ay;
		Board_A_IMU->Calc_Data.Az -= Board_A_IMU->Offset.Az;
		Board_A_IMU->Calc_Data.Gx -= Board_A_IMU->Offset.Gx;
		Board_A_IMU->Calc_Data.Gy -= Board_A_IMU->Offset.Gy;
		Board_A_IMU->Calc_Data.Gz -= Board_A_IMU->Offset.Gz;
	}
}

void Board_A_IMU_Calc_Angle(Board_A_IMU_t *Board_A_IMU)
{
	//Use fusion to do the AHRS estimation
	const FusionVector Board_A_IMU_Accel = {Board_A_IMU->Calc_Data.Ax, Board_A_IMU->Calc_Data.Ay, Board_A_IMU->Calc_Data.Az};
	const FusionVector Board_A_IMU_Gyro = {Board_A_IMU->Calc_Data.Gx, Board_A_IMU->Calc_Data.Gy, Board_A_IMU->Calc_Data.Gz}; 

	#ifdef BOARD_A_IMU_USE_MAGN
	const FusionVector Board_A_IMU_Magn = {Board_A_IMU->Calc_Data.Mx, Board_A_IMU->Calc_Data.My, Board_A_IMU->Calc_Data.Mz};
	FusionAhrsUpdate(&Board_A_IMU_AHRS, Board_A_IMU_Gyro, Board_A_IMU_Accel, Board_A_IMU_Magn, Board_A_IMU->Sample.Period);
	#endif
	
	#ifndef BOARD_A_IMU_USE_MAGN
	FusionAhrsUpdateNoMagnetometer(&Board_A_IMU_AHRS, Board_A_IMU_Gyro, Board_A_IMU_Accel, Board_A_IMU->Sample.Period);
	#endif
	
	const FusionEuler Board_A_IMU_Euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&Board_A_IMU_AHRS));
	
	Board_A_IMU->Export_Data.Prev_Yaw = Board_A_IMU->Export_Data.Yaw;
	
	Board_A_IMU->Export_Data.Yaw = Board_A_IMU_Euler.angle.yaw;
	Board_A_IMU->Export_Data.Pitch = Board_A_IMU_Euler.angle.pitch;
	Board_A_IMU->Export_Data.Roll = Board_A_IMU_Euler.angle.roll;
	Board_A_IMU->Export_Data.Gyro_Yaw = Board_A_IMU->Calc_Data.Gx / 6.0f; // degree/s to RPM
	Board_A_IMU->Export_Data.Gyro_Pitch = Board_A_IMU->Calc_Data.Gy / 6.0f; // degree/s to RPM
	Board_A_IMU->Export_Data.Gyro_Roll = Board_A_IMU->Calc_Data.Gz / 6.0f; // degree/s to RPM
	Board_A_IMU->Export_Data.Temperature = Board_A_IMU->Calc_Data.Temperature;
	
	//Record number of turns
	if((Board_A_IMU->Export_Data.Yaw - Board_A_IMU->Export_Data.Prev_Yaw) < - 300)
		Board_A_IMU->Export_Data.Turn_Count--;
	else if((Board_A_IMU->Export_Data.Yaw - Board_A_IMU->Export_Data.Prev_Yaw) > 300)
		Board_A_IMU->Export_Data.Turn_Count++;
	
	Board_A_IMU->Export_Data.Total_Yaw = Board_A_IMU->Export_Data.Yaw - 360.0f * Board_A_IMU->Export_Data.Turn_Count;
}

void Board_A_IMU_Reset(Board_A_IMU_t *Board_A_IMU)
{
	Board_A_IMU->Export_Data.Prev_Yaw = Board_A_IMU->Export_Data.Yaw;
	Board_A_IMU->Export_Data.Total_Yaw = 0;
	Board_A_IMU->Export_Data.Turn_Count = 0;
}
