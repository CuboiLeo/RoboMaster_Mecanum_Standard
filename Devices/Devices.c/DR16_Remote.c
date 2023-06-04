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
 
 void DR16_Handler(UART_HandleTypeDef *huart);
 void DR16_USART_Receive_DMA(UART_HandleTypeDef *huartx);
 static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);
 void DR16_Data_Process(uint8_t *pData);
 void Keyboard_Data_Process(void);
 void Key_State_Detect(Key_State *Key);
 void Key_State_Clear(Key_State *Key);
 void Check_DR16(void);
 DR16_Export_Data_t DR16_Export_Data = {0};
 DR16_Func_t DR16_Func = DR16_Func_GroundInit;
 #undef DR16_Func_GroundInit
 
 void DR16_Data_Process(uint8_t *pData)
 {
	 if(pData == NULL)
		 return;
	 
	 //Obtain data through communication protocol provided by DJI
	 DR16_Export_Data.Remote_Control.Joystick_Left_Vx = (((pData[2] >> 6) | (pData[3] << 2) | (pData[4] << 10)) & 0x07FF) - 1024;
	 DR16_Export_Data.Remote_Control.Joystick_Left_Vy = (((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF) - 1024;
	 DR16_Export_Data.Remote_Control.Joystick_Right_Vx = ((pData[0] | (pData[1] << 8)) & 0x07FF) - 1024;
	 DR16_Export_Data.Remote_Control.Joystick_Right_Vy = (((pData[1] >> 3) | (pData[2] << 5)) & 0x07FF) - 1024;
	 DR16_Export_Data.Remote_Control.Dial_Wheel = ((pData[16] | (pData[17] << 8)) & 0x07FF) - 1024;
	 DR16_Export_Data.Remote_Control.Left_Switch = ((pData[5] >> 4) & 0x000C) >> 2;
	 DR16_Export_Data.Remote_Control.Right_Switch = ((pData[5] >> 4) & 0x0003);
	 DR16_Export_Data.Mouse.x = (pData[6]) | (pData[7] << 8);
	 DR16_Export_Data.Mouse.y = (pData[8]) | (pData[9] << 8);
	 DR16_Export_Data.Mouse.Left_Click = pData[12];
	 DR16_Export_Data.Mouse.Right_Click = pData[13];
	 DR16_Export_Data.Keyboard.Buffer = pData[14] | (pData[15] << 8);
	 
	 //Prevent zero deviation
	 if(DR16_Export_Data.Remote_Control.Joystick_Left_Vx <= 20 && DR16_Export_Data.Remote_Control.Joystick_Left_Vx >= -20)
		 DR16_Export_Data.Remote_Control.Joystick_Left_Vx = 0;
	 if(DR16_Export_Data.Remote_Control.Joystick_Left_Vy <= 20 && DR16_Export_Data.Remote_Control.Joystick_Left_Vy >= -20)
		 DR16_Export_Data.Remote_Control.Joystick_Left_Vy = 0;
	 if(DR16_Export_Data.Remote_Control.Joystick_Right_Vx <= 20 && DR16_Export_Data.Remote_Control.Joystick_Right_Vx >= -20)
		 DR16_Export_Data.Remote_Control.Joystick_Right_Vx = 0;
	 if(DR16_Export_Data.Remote_Control.Joystick_Right_Vy <= 20 && DR16_Export_Data.Remote_Control.Joystick_Right_Vy >= -20)
		 DR16_Export_Data.Remote_Control.Joystick_Right_Vy = 0;
	 if(DR16_Export_Data.Remote_Control.Dial_Wheel <= 20 && DR16_Export_Data.Remote_Control.Dial_Wheel >= -20)
		 DR16_Export_Data.Remote_Control.Dial_Wheel = 0;
	 
	 Keyboard_Data_Process();
	 DR16_Export_Data.Info_Update_Frame++;
 }
 
/**	<Keyboard Control> Pretty cool hah?
 * ©°©¤©¤©¤©´   ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´
 * ©¦Esc©¦   ©¦ F1©¦ F2©¦ F3©¦ F4©¦ ©¦ F5©¦ F6©¦ F7©¦ F8©¦ ©¦ F9©¦F10©¦F11©¦F12©¦ ©¦P/S©¦S L©¦P/B©¦  ©°©´    ©°©´    ©°©´
 * ©¸©¤©¤©¤©¼   ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼  ©¸©¼    ©¸©¼    ©¸©¼
 * ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´ ©°©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©Ð©¤©¤©¤©´
 * ©¦~ `©¦! 1©¦@ 2©¦# 3©¦$ 4©¦% 5©¦^ 6©¦& 7©¦* 8©¦( 9©¦) 0©¦_ -©¦+ =©¦ BacSp ©¦ ©¦Ins©¦Hom©¦PUp©¦ ©¦N L©¦ / ©¦ * ©¦ - ©¦
 * ©À©¤©¤©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©¤©¤©¤©¤©È ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È
 * ©¦ Tab ©¦ Q ©¦ W ©¦ E ©¦ R ©¦ T ©¦ Y ©¦ U ©¦ I ©¦ O ©¦ P ©¦{ [©¦} ]©¦ | \ ©¦ ©¦Del©¦End©¦PDn©¦ ©¦ 7 ©¦ 8 ©¦ 9 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©Ð©¤©¤©Ø©¤©¤©¤©¤©¤©È ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È + ©¦
 * ©¦ Caps ©¦ A ©¦ S ©¦ D ©¦ F ©¦ G ©¦ H ©¦ J ©¦ K ©¦ L ©¦: ;©¦" '©¦ Enter  ©¦               ©¦ 4 ©¦ 5 ©¦ 6 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©Ð©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©È     ©°©¤©¤©¤©´     ©À©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©È
 * ©¦ Shift  ©¦ Z ©¦ X ©¦ C ©¦ V ©¦ B ©¦ N ©¦ M ©¦< ,©¦> .©¦? /©¦  Shift   ©¦     ©¦ ¡ü ©¦     ©¦ 1 ©¦ 2 ©¦ 3 ©¦   ©¦
 * ©À©¤©¤©¤©¤©¤©Ð©¤©¤©Ø©¤©Ð©¤©Ø©¤©¤©Ð©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©Ð©Ø©¤©¤©¤©à©¤©¤©¤©Ø©Ð©¤©¤©¤©¤©Ð©¤©¤©¤©¤©È ©°©¤©¤©¤©à©¤©¤©¤©à©¤©¤©¤©´ ©À©¤©¤©¤©Ø©¤©¤©¤©à©¤©¤©¤©È E©¦©¦
 * ©¦ Ctrl©¦    ©¦Alt ©¦         Space         ©¦ Alt©¦    ©¦    ©¦Ctrl©¦ ©¦ ¡û ©¦ ¡ý ©¦ ¡ú ©¦ ©¦   0   ©¦ . ©¦¡û©¤©¼©¦
 * ©¸©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¼ ©¸©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼ ©¸©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©Ø©¤©¤©¤©¼
 */
 
 void Keyboard_Data_Process(void)
 {
	 //Detect what key is pressed and what state it is in
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_W)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_W);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_W);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_S)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_S);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_S);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_A)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_A);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_A);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_D)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_D);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_D);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_SHIFT)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_Shift);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_Shift);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_CTRL)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_Ctrl);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_Ctrl);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_Q)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_Q);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_Q);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_E)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_E);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_E);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_R)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_R);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_R);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_F)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_F);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_F);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_G)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_G);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_G);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_Z)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_Z);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_Z);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_X)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_X);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_X);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_C)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_C);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_C);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_V)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_V);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_V);
	 
	 if(DR16_Export_Data.Keyboard.Buffer & KEY_B)
		 Key_State_Detect(&DR16_Export_Data.Keyboard.Press_B);
	 else
		 Key_State_Clear(&DR16_Export_Data.Keyboard.Press_B); 
 }


void Key_State_Detect(Key_State *Key)
 {
	 Key->State_Count++;
	 Key->Current_State = 1;
	 if(Key->State_Count > 1)
	 {
		 Key->Prev_State = 1;
		 Key->Hold_Flag = 1;
	 }
	 if(Key->Current_State == 1 && Key->Prev_State == 0)
		 Key->Switch_Flag = 1;
	 else
		 Key->Switch_Flag = 0;
 }
 
 void Key_State_Clear(Key_State *Key)
 {
	 Key->Hold_Flag = 0;
	 Key->State_Count = 0;
	 Key->Switch_Flag = 0;
	 Key->Current_State = 0;
	 Key->Prev_State = 0;
 }
 
 void Check_DR16(void)
 {
	 if(DR16_Export_Data.Info_Update_Frame < 1)
		 DR16_Export_Data.Offline_Flag = 1;
	 else
		 DR16_Export_Data.Offline_Flag = 0;
 }

 //Enable USART DMA Transfer
 static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
 {
	 if(huart->RxState == HAL_UART_STATE_READY)
	 {
		 if((pData == NULL) || (Size == 0))
			 return HAL_ERROR;
		 huart->pRxBuffPtr = pData;
		 huart->RxXferSize = Size;
		 huart->ErrorCode = HAL_UART_ERROR_NONE;
		 
		 HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR,(uint32_t)pData,Size);
		 SET_BIT(huart->Instance->CR3,USART_CR3_DMAR);
	 }
	 else
		 return HAL_BUSY;
	 return HAL_OK;
 }
 
 //Receive data if pass verification
 void DR16_Handler(UART_HandleTypeDef *huart)
 {
	 __HAL_DMA_DISABLE(huart->hdmarx);
	 if(__HAL_DMA_GET_COUNTER(huart->hdmarx) == DR16_BUFFER_LAST_NUMBER)
		 DR16_Data_Process(DR16_Export_Data.DR16_Buffer);
	 __HAL_DMA_SET_COUNTER(huart->hdmarx,DR16_BUFFER_NUMBER);
	 __HAL_DMA_ENABLE(huart->hdmarx);
 }
 
 void DR16_USART_Receive_DMA(UART_HandleTypeDef *huartx)
 {
	 __HAL_UART_CLEAR_IDLEFLAG(huartx);
	 __HAL_UART_ENABLE(huartx);
	 __HAL_UART_ENABLE_IT(huartx,UART_IT_IDLE);
	 USART_Receive_DMA_NO_IT(huartx,DR16_Export_Data.DR16_Buffer,DR16_BUFFER_NUMBER);
 }
