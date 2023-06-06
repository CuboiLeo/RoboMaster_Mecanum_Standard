/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "Board_A_IMU.h"
#include "IMU_Temp_Control.h"
#include "DR16_Remote.h"
#include "M3508_Motor.h"
#include "GM6020_Motor.h"
#include "M2006_Motor.h"
#include "Super_Capacitor.h"
#include "Robot_Control.h"
#include "MPU6050_IMU.h"
#include "WT901_IMU.h"
#include "Fusion.h"
#include "Referee_System.h"
#include "Buzzer.h"
#include "Jetson_Tx2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Task_IMUHandle;
osThreadId Task_InitHandle;
osThreadId Task_CAN_SendHandle;
osThreadId Task_CAN1_RecHandle;
osThreadId Task_CAN2_RecHandle;
osThreadId Task_Robot_CtrlHandle;
osMessageQId CAN1_ReceiveHandle;
osMessageQId CAN2_ReceiveHandle;
osMessageQId CAN_SendHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartIMUTask(void const * argument);
void General_Init(void const * argument);
void CAN_Send_ALL(void const * argument);
void CAN1_Rec(void const * argument);
void CAN2_Rec(void const * argument);
void Robot_Control(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of CAN1_Receive */
  osMessageQDef(CAN1_Receive, 32, CAN_Export_Data_t);
  CAN1_ReceiveHandle = osMessageCreate(osMessageQ(CAN1_Receive), NULL);

  /* definition and creation of CAN2_Receive */
  osMessageQDef(CAN2_Receive, 32, CAN_Export_Data_t);
  CAN2_ReceiveHandle = osMessageCreate(osMessageQ(CAN2_Receive), NULL);

  /* definition and creation of CAN_Send */
  osMessageQDef(CAN_Send, 32, CAN_Send_Data_t);
  CAN_SendHandle = osMessageCreate(osMessageQ(CAN_Send), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_IMU */
  osThreadDef(Task_IMU, StartIMUTask, osPriorityNormal, 0, 384);
  Task_IMUHandle = osThreadCreate(osThread(Task_IMU), NULL);

  /* definition and creation of Task_Init */
  osThreadDef(Task_Init, General_Init, osPriorityHigh, 0, 128);
  Task_InitHandle = osThreadCreate(osThread(Task_Init), NULL);

  /* definition and creation of Task_CAN_Send */
  osThreadDef(Task_CAN_Send, CAN_Send_ALL, osPriorityHigh, 0, 256);
  Task_CAN_SendHandle = osThreadCreate(osThread(Task_CAN_Send), NULL);

  /* definition and creation of Task_CAN1_Rec */
  osThreadDef(Task_CAN1_Rec, CAN1_Rec, osPriorityHigh, 0, 256);
  Task_CAN1_RecHandle = osThreadCreate(osThread(Task_CAN1_Rec), NULL);

  /* definition and creation of Task_CAN2_Rec */
  osThreadDef(Task_CAN2_Rec, CAN2_Rec, osPriorityHigh, 0, 256);
  Task_CAN2_RecHandle = osThreadCreate(osThread(Task_CAN2_Rec), NULL);

  /* definition and creation of Task_Robot_Ctrl */
  osThreadDef(Task_Robot_Ctrl, Robot_Control, osPriorityRealtime, 0, 640);
  Task_Robot_CtrlHandle = osThreadCreate(osThread(Task_Robot_Ctrl), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartIMUTask */
/**
  * @brief  Function implementing the Task_IMU thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartIMUTask */
void StartIMUTask(void const * argument)
{
  /* USER CODE BEGIN StartIMUTask */
	portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  const TickType_t TimeIncrement = pdMS_TO_TICKS(1);
  /* Infinite loop */
  for(;;)
  {
		//Board_A_IMU_Func.Board_A_IMU_Calibrate(&Board_A_IMU);
		Board_A_IMU_Func.Board_A_IMU_Read_Data(&Board_A_IMU);
		Board_A_IMU_Func.Board_A_IMU_Calc_Angle(&Board_A_IMU);
		IMU_Temp_Control_Func.Board_A_IMU_Temp_Control();
		
		#ifdef USE_MPU6050
		MPU6050_IMU_Func.MPU6050_IMU_Calibrate(&MPU6050_IMU);
		MPU6050_IMU_Func.MPU6050_IMU_Read_Data(&MPU6050_IMU);
		MPU6050_IMU_Func.MPU6050_IMU_Calc_Angle(&MPU6050_IMU);
		#endif
		
    vTaskDelayUntil(&xLastWakeTime, TimeIncrement);
  }
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_General_Init */
/**
* @brief Function implementing the Task_Init thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_General_Init */
void General_Init(void const * argument)
{
  /* USER CODE BEGIN General_Init */
  /* Infinite loop */
	Buzzer_Func.Buzzer_Init();
	//Buzzer_Func.Buzzer_Robot_Initializing();
  IMU_Temp_Control_Func.Board_A_IMU_Temp_Control_Init();
	Board_A_IMU_Func.Board_A_IMU_Init();	
	#ifdef USE_MPU6050
	MPU6050_IMU_Func.MPU6050_IMU_Init();
	#endif
	FusionAhrsInitialise(&MPU6050_IMU_AHRS);
	FusionAhrsInitialise(&Board_A_IMU_AHRS);
	DR16_Func.DR16_USART_Receive_DMA(&huart1);
	Tx2_Func.Jetson_Tx2_Initialization();
	Referee_System_Func.Referee_UART_Receive_Interrupt(&huart6, Referee_System.Buffer, REFEREE_BUFFER_LEN);
	CAN_Func.CAN_IT_Init(&hcan1, CAN1_Type);
  CAN_Func.CAN_IT_Init(&hcan2, CAN2_Type);
	Gimbal_Func.Gimbal_Init();
	Shooting_Func.Shooting_Init();
	vTaskDelete(NULL);
  /* USER CODE END General_Init */
}

/* USER CODE BEGIN Header_CAN_Send_ALL */
/**
* @brief Function implementing the Task_CAN_Send thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_Send_ALL */
void CAN_Send_ALL(void const * argument)
{
  /* USER CODE BEGIN CAN_Send_ALL */
	CAN_Send_Data_t CAN_Send_Data;
  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(CAN_SendHandle, &CAN_Send_Data, portMAX_DELAY);
    HAL_CAN_AddTxMessage(CAN_Send_Data.Canx, &CAN_Send_Data.CAN_TxHeader, CAN_Send_Data.CANx_Send_RxMessage, (uint32_t *)CAN_TX_MAILBOX0);
  }
  /* USER CODE END CAN_Send_ALL */
}

/* USER CODE BEGIN Header_CAN1_Rec */
/**
* @brief Function implementing the Task_CAN1_Rec thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN1_Rec */
void CAN1_Rec(void const * argument)
{
  /* USER CODE BEGIN CAN1_Rec */
	CAN_Export_Data_t CAN_Export_Data;
	uint32_t ID;
  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(CAN1_ReceiveHandle, &CAN_Export_Data, portMAX_DELAY);
		ID = CAN_Export_Data.CAN_RxHeader.StdId;
		if(ID == SUPERCAP_ID)
			Super_Capacitor_Func.Super_Capacitor_Get_Data(CAN_Export_Data);
		else if(ID >= M3508_CHASSIS_START_ID && ID <= M3508_CHASSIS_END_ID)
			M3508_Func.M3508_Chassis_Get_Data(CAN_Export_Data);
		else if(ID == GM6020_YAW_ID)
			GM6020_Func.GM6020_Yaw_Get_Data(CAN_Export_Data);
		else if(ID == GM6020_PITCH_ID)
			GM6020_Func.GM6020_Pitch_Get_Data(CAN_Export_Data);
		Monitor_CAN1.Info_Update_Frame++;
  }
  /* USER CODE END CAN1_Rec */
}

/* USER CODE BEGIN Header_CAN2_Rec */
/**
* @brief Function implementing the Task_CAN2_Rec thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN2_Rec */
void CAN2_Rec(void const * argument)
{
  /* USER CODE BEGIN CAN2_Rec */
	CAN_Export_Data_t CAN_Export_Data;
	uint32_t ID;
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(CAN2_ReceiveHandle, &CAN_Export_Data, portMAX_DELAY);
		ID = CAN_Export_Data.CAN_RxHeader.StdId;
    if(ID == M3508_FRIC_WHEEL_LEFT_ID || ID == M3508_FRIC_WHEEL_RIGHT_ID)
			M3508_Func.M3508_Fric_Wheel_Get_Data(CAN_Export_Data);
		else if(ID == M2006_TRIGGER_ID)
			M2006_Func.M2006_Trigger_Get_Data(CAN_Export_Data);
		
		Monitor_CAN2.Info_Update_Frame++;
  }
  /* USER CODE END CAN2_Rec */
}

/* USER CODE BEGIN Header_Robot_Control */
/**
* @brief Function implementing the Task_Robot_Ctrl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Robot_Control */
void Robot_Control(void const * argument)
{
  /* USER CODE BEGIN Robot_Control */
	portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  const TickType_t TimeIncrement = pdMS_TO_TICKS(2);
  /* Infinite loop */
  for(;;)
  {
    Robot_Control_Func.Robot_Control_Start();
		vTaskDelayUntil(&xLastWakeTime, TimeIncrement);
  }
  /* USER CODE END Robot_Control */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
