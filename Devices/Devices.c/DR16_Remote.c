/**
 * @file DR16_Remote.c
 * @author Leo Liu
 * @brief communicate and obtain data from DR16
 * @version 1.0
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
 #include "DR16_Remote.h"
 
 void DR16_Handler(UART_HandleTypeDef *huartx);
 void DR16_USART_Receive_DMA(UART_HandleTypeDef *huartx);
 static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huartx, uint8_t *pData, uint32_t Size);
 void DR16_Data_Process(uint8_t *pData);
 void Keyboard_Data_Process(void);
 uint8_t Key_State_Detect(Key_State Key);
 
 DR16_Data_t DR16_Data;
 
 void DR16_Data_Process(uint8_t *pData)
 {
	 if(pData == NULL)
		 return;
	 
	 DR16_Data.Remote_Control.Joystick_Left_Vx = (((pData[2] >> 6) | (pData[3] << 2) | (pData[4] << 10)) & 0x07FF) - 1024;
	 DR16_Data.Remote_Control.Joystick_Left_Vy = (((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF) - 1024;
	 DR16_Data.Remote_Control.Joystick_Right_Vx = ((pData[0] | (pData[1] << 8)) & 0x07FF) - 1024;
	 DR16_Data.Remote_Control.Joystick_Right_Vy = (((pData[1] >> 3) | (pData[2] << 5)) & 0x07FF) - 1024;
	 DR16_Data.Remote_Control.Dial_Wheel = ((pData[16] | (pData[17] << 8)) & 0x07FF) - 1024;
	 DR16_Data.Remote_Control.Left_Switch = ((pData[5] >> 4) & 0x000C) >> 2;
	 DR16_Data.Remote_Control.Right_Switch = ((pData[5] >> 4) & 0x0003);
	 DR16_Data.Mouse.x = (pData[6]) | (pData[7] << 8);
	 DR16_Data.Mouse.y = (pData[8]) | (pData[9] << 8);
	 DR16_Data.Mouse.Left_Click = pData[12];
	 DR16_Data.Mouse.Right_Click = pData[13];
	 DR16_Data.Keyboard.Buffer = pData[14] | (pData[15] << 8);
	 
	 if(DR16_Data.Remote_Control.Joystick_Left_Vx <= 20 && DR16_Data.Remote_Control.Joystick_Left_Vx >= -20)
		 DR16_Data.Remote_Control.Joystick_Left_Vx = 0;
	 if(DR16_Data.Remote_Control.Joystick_Left_Vy <= 20 && DR16_Data.Remote_Control.Joystick_Left_Vy >= -20)
		 DR16_Data.Remote_Control.Joystick_Left_Vy = 0;
	 if(DR16_Data.Remote_Control.Joystick_Right_Vx <= 20 && DR16_Data.Remote_Control.Joystick_Right_Vx >= -20)
		 DR16_Data.Remote_Control.Joystick_Right_Vx = 0;
	 if(DR16_Data.Remote_Control.Joystick_Right_Vy <= 20 && DR16_Data.Remote_Control.Joystick_Right_Vy >= -20)
		 DR16_Data.Remote_Control.Joystick_Right_Vy = 0;
	 if(DR16_Data.Remote_Control.Dial_Wheel <= 20 && DR16_Data.Remote_Control.Dial_Wheel >= -20)
		 DR16_Data.Remote_Control.Dial_Wheel = 0;
	 
	 Keyboard_Data_Process();
 }
 
 void Keyboard_Data_Process(void)
 {
	 if(DR16_Data.Keyboard.Buffer == KEY_W)
		 DR16_Data.Keyboard.Press_W.State = Key_State_Detect(DR16_Data.Keyboard.Press_W);
	 else{
		 DR16_Data.Keyboard.Press_W.State = 0;
		 DR16_Data.Keyboard.Press_W.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_S)
		 DR16_Data.Keyboard.Press_S.State = Key_State_Detect(DR16_Data.Keyboard.Press_S);
	 else{
		 DR16_Data.Keyboard.Press_S.State = 0;
		 DR16_Data.Keyboard.Press_S.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_A)
		 DR16_Data.Keyboard.Press_A.State = Key_State_Detect(DR16_Data.Keyboard.Press_A);
	 else{
		 DR16_Data.Keyboard.Press_A.State = 0;
		 DR16_Data.Keyboard.Press_A.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_D)
		 DR16_Data.Keyboard.Press_D.State = Key_State_Detect(DR16_Data.Keyboard.Press_D);
	 else{
		 DR16_Data.Keyboard.Press_D.State = 0;
		 DR16_Data.Keyboard.Press_D.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_SHIFT)
		 DR16_Data.Keyboard.Press_Shift.State = Key_State_Detect(DR16_Data.Keyboard.Press_Shift);
	 else{
		 DR16_Data.Keyboard.Press_Shift.State = 0;
		 DR16_Data.Keyboard.Press_Shift.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_CTRL)
		 DR16_Data.Keyboard.Press_Ctrl.State = Key_State_Detect(DR16_Data.Keyboard.Press_Ctrl);
	 else{
		 DR16_Data.Keyboard.Press_Ctrl.State = 0;
		 DR16_Data.Keyboard.Press_Ctrl.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_Q)
		 DR16_Data.Keyboard.Press_Q.State = Key_State_Detect(DR16_Data.Keyboard.Press_Q);
	 else{
		 DR16_Data.Keyboard.Press_Q.State = 0;
		 DR16_Data.Keyboard.Press_Q.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_E)
		 DR16_Data.Keyboard.Press_E.State = Key_State_Detect(DR16_Data.Keyboard.Press_E);
	 else{
		 DR16_Data.Keyboard.Press_E.State = 0;
		 DR16_Data.Keyboard.Press_E.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_R)
		 DR16_Data.Keyboard.Press_R.State = Key_State_Detect(DR16_Data.Keyboard.Press_R);
	 else{
		 DR16_Data.Keyboard.Press_R.State = 0;
		 DR16_Data.Keyboard.Press_R.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_F)
		 DR16_Data.Keyboard.Press_F.State = Key_State_Detect(DR16_Data.Keyboard.Press_F);
	 else{
		 DR16_Data.Keyboard.Press_F.State = 0;
		 DR16_Data.Keyboard.Press_F.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_G)
		 DR16_Data.Keyboard.Press_G.State = Key_State_Detect(DR16_Data.Keyboard.Press_G);
	 else{
		 DR16_Data.Keyboard.Press_G.State = 0;
		 DR16_Data.Keyboard.Press_G.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_Z)
		 DR16_Data.Keyboard.Press_Z.State = Key_State_Detect(DR16_Data.Keyboard.Press_Z);
	 else{
		 DR16_Data.Keyboard.Press_Z.State = 0;
		 DR16_Data.Keyboard.Press_Z.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_X)
		 DR16_Data.Keyboard.Press_X.State = Key_State_Detect(DR16_Data.Keyboard.Press_X);
	 else{
		 DR16_Data.Keyboard.Press_X.State = 0;
		 DR16_Data.Keyboard.Press_X.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_C)
		 DR16_Data.Keyboard.Press_C.State = Key_State_Detect(DR16_Data.Keyboard.Press_C);
	 else{
		 DR16_Data.Keyboard.Press_C.State = 0;
		 DR16_Data.Keyboard.Press_C.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_V)
		 DR16_Data.Keyboard.Press_V.State = Key_State_Detect(DR16_Data.Keyboard.Press_V);
	 else{
		 DR16_Data.Keyboard.Press_V.State = 0;
		 DR16_Data.Keyboard.Press_V.State_Count = 0;
	 }
	 
	 if(DR16_Data.Keyboard.Buffer == KEY_B)
		 DR16_Data.Keyboard.Press_B.State = Key_State_Detect(DR16_Data.Keyboard.Press_B);
	 else{
		 DR16_Data.Keyboard.Press_B.State = 0;
		 DR16_Data.Keyboard.Press_B.State_Count = 0;
	 }
 }
 
 uint8_t Key_State_Detect(Key_State Key)
 {
	 uint8_t State;
	 Key.State_Count++;
	 
	 if(Key.State_Count >= TIME_KEY_SINGLECLICK && Key.State_Count <= TIME_KEY_HOLD)
		 State = Key.Single_Click_Flag;
	 else if(Key.State_Count > TIME_KEY_HOLD)
		 State = Key.Hold_Flag;
	 else
		 State = 0;
	 
	 return State;
 }
 
 static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huartx, uint8_t *pData, uint32_t Size)
 {
	 if(huartx->RxState == HAL_UART_STATE_READY)
	 {
		 if((pData == NULL) || (Size == 0))
			 return HAL_ERROR;
		 huartx->pRxBuffPtr = pData;
		 huartx->RxXferSize = Size;
		 huartx->ErrorCode = HAL_UART_ERROR_NONE;
		 
		 HAL_DMA_Start(huartx->hdmarx, (uint32_t)&huartx->Instance->DR,(uint32_t)pData,Size);
		 SET_BIT(huartx->Instance->CR3,USART_CR3_DMAR);
	 }
	 else
		 return HAL_BUSY;
	 return HAL_OK;
 }
 void DR16_Handler(UART_HandleTypeDef *huartx)
 {
	 __HAL_DMA_DISABLE(huartx->hdmarx);
	 if(__HAL_DMA_GET_COUNTER(huartx->hdmarx) == DR16_BUFFER_LAST_NUMBER)
		 DR16_Data_Process(DR16_Data.DR16_Buffer);
	 __HAL_DMA_SET_COUNTER(huartx->hdmarx,DR16_BUFFER_NUMBER);
	 __HAL_DMA_ENABLE(huartx->hdmarx);
 }
 
 void DR16_USART_Receive_DMA(UART_HandleTypeDef *huartx)
 {
	 __HAL_UART_CLEAR_IDLEFLAG(huartx);
	 __HAL_UART_ENABLE(huartx);
	 __HAL_UART_ENABLE_IT(huartx,UART_IT_IDLE);
	 USART_Receive_DMA_NO_IT(huartx,DR16_Data.DR16_Buffer,DR16_BUFFER_NUMBER);
 }
