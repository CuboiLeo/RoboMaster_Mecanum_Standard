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
#include "MPU6500_Reg.h"
#include "IST8310_Reg.h"
#include "Filters.h"
#include "User_Defined_Math.h"

#define BOARD_A_IMU_SPI hspi5
#define SPI_NSS_LOW HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)
#define SPI_NSS_HIGH HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)

volatile float q0 = 1.0f;
volatile float q1 = 0.0f;
volatile float q2 = 0.0f;
volatile float q3 = 0.0f;
volatile float exInt, eyInt, ezInt; /* error integral */
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;
volatile uint32_t last_update, now_update; /* Sampling cycle count, ubit ms */
static uint8_t tx, rx;
static uint8_t tx_buff[14] = {0xff};
static uint8_t mpu_buff[15]; /* buffer to save IMU_Calculated_Data raw data */
static uint8_t ist_buff[6];  /* buffer to save IST8310 raw data */
uint32_t IMU_Offset[IMU_OFFSET_NUMBER];
uint32_t IMUwriteFlashData[IMU_OFFSET_NUMBER] = {0};

IMU_Original_Data_t IMU_Original_Data;
IMU_Calculated_Data_t IMU_Calculated_Data;
IMU_Export_t IMU_Export;
Board_A_IMU_Func_t Board_A_IMU_Func;

void Board_A_IMU_Init(void);
static uint8_t Board_A_IMU_Device_Init(void);
static HAL_StatusTypeDef Board_A_IMU_Read_Bytes(uint8_t const regAddr, uint8_t *pData, uint8_t len);
static uint8_t Board_A_IMU_Read_Byte(uint8_t const reg);
static uint8_t Board_A_IMU_Write_Byte(uint8_t const reg, uint8_t const data);
static uint8_t Board_A_IMU_Set_Gyro_FSR(uint8_t fsr);
static uint8_t Board_A_IMU_Set_ACCE_FSR(uint8_t fsr);
static uint8_t IST8310_Init(void);;
static void IST8310_Write_Byte(uint8_t addr, uint8_t data);
static void Board_A_IMU_I2C_Config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
static uint8_t IST8310_Read_Byte(uint8_t addr);
static void IST8310_Get_Data(uint8_t *buff);
void Quaternion_Init(void);
void Board_A_IMU_Get_Data(void);
void Board_A_IMU_Attitude_Update(void);
void Board_A_IMU_Export_Data(IMU_Calculated_Data_t *IMU_Calculated_Data, IMU_Export_t *IMU_Export);
void Board_A_IMU_Reset(IMU_Export_t *IMU_Export);
void Board_A_IMU_Check(void);

void Board_A_IMU_Init(void)
{
    Board_A_IMU_Device_Init();
    Quaternion_Init();
}

static uint8_t Board_A_IMU_Device_Init(void)
{
    HAL_Delay(100);

    uint8_t i = 0;
    uint8_t MPU6500_Init_Data[10][2] = {
        {MPU6500_PWR_MGMT_1, 0x80},     /* Reset Device */
        {MPU6500_PWR_MGMT_1, 0x03},     /* Clock Source - Gyro-Z */
        {MPU6500_PWR_MGMT_2, 0x00},     /* Enable Acc & Gyro */
        {MPU6500_CONFIG, 0x04},         /* LPF 41Hz */
        {MPU6500_GYRO_CONFIG, 0x18},    /* +-2000dps */
        {MPU6500_ACCEL_CONFIG, 0x10},   /* +-8G */
        {MPU6500_ACCEL_CONFIG_2, 0x02}, /* enable LowPassFilter  Set Acc LPF */
        {MPU6500_USER_CTRL, 0x20},
    }; /* Enable AUX */
    for (i = 0; i < 10; i++)
    {
        Board_A_IMU_Write_Byte(MPU6500_Init_Data[i][0], MPU6500_Init_Data[i][1]);
        HAL_Delay(1);
    }

    Board_A_IMU_Set_Gyro_FSR(3);
    Board_A_IMU_Set_ACCE_FSR(2);

    IST8310_Init();
    IMU_Export.IMU_Init_Condition = 1;
		
    return 0;
}

static HAL_StatusTypeDef Board_A_IMU_Read_Bytes(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
    HAL_StatusTypeDef InfoUpdateFlag = HAL_ERROR;
    SPI_NSS_LOW;
    tx = regAddr | 0x80;
    tx_buff[0] = tx;
    HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, &tx, &rx, 1, 55);
    InfoUpdateFlag = HAL_SPI_TransmitReceive(&BOARD_A_IMU_SPI, tx_buff, pData, len, 55);
    SPI_NSS_HIGH;
    return InfoUpdateFlag;
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

static uint8_t Board_A_IMU_Set_Gyro_FSR(uint8_t fsr)
{
    return Board_A_IMU_Write_Byte(MPU6500_GYRO_CONFIG, fsr << 3);
}

static uint8_t Board_A_IMU_Set_ACCE_FSR(uint8_t fsr)
{
    return Board_A_IMU_Write_Byte(MPU6500_ACCEL_CONFIG, fsr << 3);
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

static void IST8310_Get_Data(uint8_t *buff)
{
    Board_A_IMU_Read_Bytes(MPU6500_EXT_SENS_DATA_00, buff, 6);
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

void Quaternion_Init(void)
{
    int16_t hx, hy;

    hx = IMU_Calculated_Data.mx;
    hy = IMU_Calculated_Data.my;

#ifdef BOARD_DOWN
    if (hx < 0 && hy < 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = -0.005;
            q1 = -0.199;
            q2 = 0.979;
            q3 = -0.0089;
        }
        else
        {
            q0 = -0.008;
            q1 = -0.555;
            q2 = 0.83;
            q3 = -0.002;
        }
    }
    else if (hx < 0 && hy > 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = 0.005;
            q1 = -0.199;
            q2 = -0.978;
            q3 = 0.012;
        }
        else
        {
            q0 = 0.005;
            q1 = -0.553;
            q2 = -0.83;
            q3 = -0.0023;
        }
    }
    else if (hx > 0 && hy > 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = 0.0012;
            q1 = -0.978;
            q2 = -0.199;
            q3 = -0.005;
        }
        else
        {
            q0 = 0.0023;
            q1 = -0.83;
            q2 = -0.553;
            q3 = 0.0023;
        }
    }
    else if (hx > 0 && hy < 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = 0.0025;
            q1 = 0.978;
            q2 = -0.199;
            q3 = 0.008;
        }
        else
        {
            q0 = 0.0025;
            q1 = 0.83;
            q2 = -0.56;
            q3 = 0.0045;
        }
    }
#else
    if (hx < 0 && hy < 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = 0.195;
            q1 = -0.015;
            q2 = 0.0043;
            q3 = 0.979;
        }
        else
        {
            q0 = 0.555;
            q1 = -0.015;
            q2 = 0.006;
            q3 = 0.829;
        }
    }
    else if (hx < 0 && hy > 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = -0.193;
            q1 = -0.009;
            q2 = -0.006;
            q3 = 0.979;
        }
        else
        {
            q0 = -0.552;
            q1 = -0.0048;
            q2 = -0.0115;
            q3 = 0.8313;
        }
    }
    else if (hx > 0 && hy > 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = -0.9785;
            q1 = 0.008;
            q2 = -0.02;
            q3 = 0.195;
        }
        else
        {
            q0 = -0.9828;
            q1 = 0.002;
            q2 = -0.0167;
            q3 = 0.5557;
        }
    }
    else if (hx > 0 && hy < 0)
    {
        if (fabs(hx / hy) >= 1)
        {
            q0 = -0.979;
            q1 = 0.0116;
            q2 = -0.0167;
            q3 = -0.195;
        }
        else
        {
            q0 = -0.83;
            q1 = 0.014;
            q2 = -0.012;
            q3 = -0.556;
        }
    }
#endif
}

void Board_A_IMU_Get_Data(void)
{
    IMU_Export.InfoUpdateFlag = Board_A_IMU_Read_Bytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);

    IMU_Original_Data.ax = mpu_buff[0] << 8 | mpu_buff[1];
    IMU_Original_Data.ay = mpu_buff[2] << 8 | mpu_buff[3];
    IMU_Original_Data.az = mpu_buff[4] << 8 | mpu_buff[5];
    IMU_Original_Data.temp = mpu_buff[6] << 8 | mpu_buff[7];

    IMU_Original_Data.Original_Gyro.gx = ((mpu_buff[8] << 8 | mpu_buff[9]) - IMU_Original_Data.Offset.gx_offset);
    IMU_Original_Data.Original_Gyro.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - IMU_Original_Data.Offset.gy_offset);
    IMU_Original_Data.Original_Gyro.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - IMU_Original_Data.Offset.gz_offset);

    IST8310_Get_Data(ist_buff);
    memcpy(&IMU_Original_Data.mx, ist_buff, 6);

    memcpy(&IMU_Calculated_Data.ax, &IMU_Original_Data.ax, 6 * sizeof(int16_t));

    IMU_Calculated_Data.temp = 21 + IMU_Original_Data.temp / 333.87f;
	
    IMU_Calculated_Data.Real_Gyro.wx = IMU_Original_Data.Original_Gyro.gx / 16.384f / 57.3f;
    IMU_Calculated_Data.Real_Gyro.wy = IMU_Original_Data.Original_Gyro.gy / 16.384f / 57.3f;
    IMU_Calculated_Data.Real_Gyro.wz = IMU_Original_Data.Original_Gyro.gz / 16.384f / 57.3f;

    IMU_Calculated_Data.Gyro.gx = IMU_Original_Data.Original_Gyro.gx;
    IMU_Calculated_Data.Gyro.gy = IMU_Original_Data.Original_Gyro.gy;
    IMU_Calculated_Data.Gyro.gz = IMU_Original_Data.Original_Gyro.gz;
}

void Board_A_IMU_AHRS_Update(void)
{
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez, halfT;
    float tempq0, tempq1, tempq2, tempq3;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    gx = IMU_Calculated_Data.Real_Gyro.wx;
    gy = IMU_Calculated_Data.Real_Gyro.wy;
    gz = IMU_Calculated_Data.Real_Gyro.wz;
    ax = IMU_Calculated_Data.ax;
    ay = IMU_Calculated_Data.ay;
    az = IMU_Calculated_Data.az;
    mx = IMU_Calculated_Data.mx;
    my = IMU_Calculated_Data.my;
    mz = IMU_Calculated_Data.mz;

    now_update = HAL_GetTick(); //ms
    halfT = ((float)(now_update - last_update) / 2000.0f);
    last_update = now_update;

    /* Fast inverse square-root */
    norm = inv_sqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;

#ifdef IST8310
    norm = inv_sqrt(mx * mx + my * my + mz * mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;
#else
    mx = 0;
    my = 0;
    mz = 0;
#endif
    /* compute reference direction of flux */
    hx = 2.0f * mx * (0.5f - q2q2 - q3q3) + 2.0f * my * (q1q2 - q0q3) + 2.0f * mz * (q1q3 + q0q2);
    hy = 2.0f * mx * (q1q2 + q0q3) + 2.0f * my * (0.5f - q1q1 - q3q3) + 2.0f * mz * (q2q3 - q0q1);
    hz = 2.0f * mx * (q1q3 - q0q2) + 2.0f * my * (q2q3 + q0q1) + 2.0f * mz * (0.5f - q1q1 - q2q2);
    bx = sqrt((hx * hx) + (hy * hy));
    bz = hz;

    /* estimated direction of gravity and flux (v and w) */
    vx = 2.0f * (q1q3 - q0q2);
    vy = 2.0f * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2.0f * bx * (0.5f - q2q2 - q3q3) + 2.0f * bz * (q1q3 - q0q2);
    wy = 2.0f * bx * (q1q2 - q0q3) + 2.0f * bz * (q0q1 + q2q3);
    wz = 2.0f * bx * (q0q2 + q1q3) + 2.0f * bz * (0.5f - q1q1 - q2q2);

    /* 
	 * error is sum of cross product between reference direction 
	 * of fields and direction measured by sensors 
	 */
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    /* PI */
    if (ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;
        ezInt = ezInt + ez * Ki * halfT;

        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

    tempq0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    tempq1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    tempq2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    tempq3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    /* normalise quaternion */
    norm = inv_sqrt(tempq0 * tempq0 + tempq1 * tempq1 + tempq2 * tempq2 + tempq3 * tempq3);
    q0 = tempq0 * norm;
    q1 = tempq1 * norm;
    q2 = tempq2 * norm;
    q3 = tempq3 * norm;
}

void Board_A_IMU_Attitude_Update(void)
{
    /* yaw    -pi----pi */
    IMU_Calculated_Data.Axis.yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3 + 180.0f;
    /* pitch  -pi/2----pi/2 */
    IMU_Calculated_Data.Axis.pit = -asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3 + 180.0f;
    /* roll   -pi----pi  */
    IMU_Calculated_Data.Axis.rol = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3 + 180.0f;
	
		Board_A_IMU_Export_Data(&IMU_Calculated_Data, &IMU_Export);
}

void Board_A_IMU_Export_Data(IMU_Calculated_Data_t *IMU_Calculated_Data, IMU_Export_t *IMU_Export)
{
    IMU_Export->temp = IMU_Calculated_Data->temp;
    for (int i = 0; i < 3; i++)
    {
        IMU_Export->Current_Angle[i] = IMU_Calculated_Data->Axis.Real_Angle[i];
        if (IMU_Calculated_Data->Axis.Real_Angle[i] - IMU_Calculated_Data->Last_Axis.Last_Angle[i] < -300)
        { 
            IMU_Calculated_Data->turnCount[i]++;
        }
        if (IMU_Calculated_Data->Last_Axis.Last_Angle[i] - IMU_Calculated_Data->Axis.Real_Angle[i] < -300)
        {
            IMU_Calculated_Data->turnCount[i]--;
        }
        IMU_Export->Total_Angle[i] = IMU_Calculated_Data->Axis.Real_Angle[i] + (360 * IMU_Calculated_Data->turnCount[i]);

        IMU_Calculated_Data->Last_Axis.Last_Angle[i] = IMU_Calculated_Data->Axis.Real_Angle[i];

        Filter_IIRLPF(&IMU_Calculated_Data->Gyro.Original_gX[i], &IMU_Export->SpeedLPF[i], gz_LpfAttFactor);
    }
}

void Board_A_IMU_Reset(IMU_Export_t *IMU_Export)
{
    for (int i = 0; i < 3; i++)
    {
        IMU_Calculated_Data.Last_Axis.Last_Angle[i] = IMU_Calculated_Data.Axis.Real_Angle[i];
        IMU_Export->Total_Angle[i] = IMU_Calculated_Data.Axis.Real_Angle[i];
        IMU_Calculated_Data.turnCount[i] = 0;
    }
}

void Board_A_IMU_Check(void)
{
    if (IMU_Export.InfoUpdateFlag != HAL_OK)
    {
        IMU_Export.OffLineFlag = 1;
    }
    else
    {
        IMU_Export.OffLineFlag = 0;
    }
}
