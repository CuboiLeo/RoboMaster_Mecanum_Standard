/**
 * @file Buzzer.c
 * @author Leo Liu
 * @brief on board buzzer
 * @version 1.0
 * @date 2022/07/25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Buzzer.h"
void Buzzer_Init(void);
void Buzzer_State(void);
void Buzzer_Robot_Initializing(void);
void Buzzer_Robot_Is_Initialized(void);
void Buzzer_Modules_Offline(void);
void Buzzer_Play_Song(void);

Buzzer_Func_t Buzzer_Func = Buzzer_Func_GroundInit;
#undef Buzzer_Func_GroundInit

void Buzzer_On(uint16_t Volume, uint16_t Duration);
void Buzzer_Choose_Note(uint16_t Note);

Buzzer_t Buzzer;

uint16_t Chinese_Poker[500][2];
uint16_t Mezase_Pokemon_Master[500][2];
uint16_t Undertale_Megalovania[500][2];
uint16_t PVZ_Grasswalk_1[500][2];
uint16_t PVZ_Grasswalk_2[500][2];
uint16_t PVZ_Graze_The_Roof[100][2];

//Enable timer's PWM function
void Buzzer_Init(void)
{
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
}

void Buzzer_State(void)
{
	Buzzer.Button_State = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	if(Buzzer.Button_State == 1 && Buzzer.Button_Prev_State == 0)
		Buzzer.Song_Number ++;
	if(Buzzer.Song_Number > 6)
		Buzzer.Song_Number = 0;
	Buzzer.Button_Prev_State = Buzzer.Button_State;
}

void Buzzer_Play_Song(void)
{
	switch(Buzzer.Song_Number)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(Chinese_Poker[i][0]);
				Buzzer_On(NORMAL_VOLUME,Chinese_Poker[i][1]);
			}
			break;
		}
		case 2:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(Mezase_Pokemon_Master[i][0]);
				Buzzer_On(NORMAL_VOLUME,Mezase_Pokemon_Master[i][1]);
			}
			break;
		}
		case 3:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(Undertale_Megalovania[i][0]);
				Buzzer_On(NORMAL_VOLUME,Undertale_Megalovania[i][1]);
			}
			break;
		}
		case 4:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(PVZ_Grasswalk_1[i][0]);
				Buzzer_On(NORMAL_VOLUME,PVZ_Grasswalk_1[i][1]);
			}
		}
		case 5:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(PVZ_Grasswalk_2[i][0]);
				Buzzer_On(NORMAL_VOLUME,PVZ_Grasswalk_2[i][1]);
			}
			break;
		}
		case 6:
		{
			for(int i = 0; i < 500; i++) 
			{
				Buzzer_Choose_Note(PVZ_Graze_The_Roof[i][0]);
				Buzzer_On(NORMAL_VOLUME,PVZ_Graze_The_Roof[i][1]);
			}
			break;
		}
	}
}

//Sound effect for initializing
void Buzzer_Robot_Initializing(void)
{
	Buzzer_Choose_Note(Low_Ti);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Low_So);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Re);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Low_So);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Re);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Mi);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Re);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Low_So);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Mi);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Re);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Low_So);
	Buzzer_On(MAX_VOLUME, 200);
	Buzzer_Choose_Note(Mid_Re);
	Buzzer_On(MAX_VOLUME, 500);
	HAL_Delay(1000);
}

//Sound effect for initialization finished
void Buzzer_Robot_Is_Initialized(void)
{
	Buzzer_Choose_Note(Mid_Do);
	Buzzer_On(MAX_VOLUME, 100);
	HAL_Delay(50);
	Buzzer_Choose_Note(Mid_Do);
	Buzzer_On(MAX_VOLUME, 100);
	HAL_Delay(50);
	Buzzer_Choose_Note(Mid_Do);
	Buzzer_On(MAX_VOLUME, 100);
	HAL_Delay(50);
	Buzzer_Choose_Note(Mid_So);
	Buzzer_On(MAX_VOLUME, 1000);
}

//Sound effect for offline modules
void Buzzer_Modules_Offline(void)
{
}

//Turn on buzzer for set amount of duration
void Buzzer_On(uint16_t Volume, uint16_t Duration)
{
	__HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, Volume);
	HAL_Delay(Duration);
	__HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, 0);
}

//Choose note, constants are calculated for desired frequency of each note
void Buzzer_Choose_Note(uint16_t Note)
{
	switch(Note)
	{
		case 0:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,0);
			break;
		}
		case Low_Do:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,3821);
			break;
		}
		case Low_Do_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,3608);
			break;
		}
		case Low_Re:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,3404);
			break;
		}
		case Low_Re_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,3214);
			break;
		}
		case Low_Mi:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,3033);
			break;
		}
		case Low_Fa:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2863);
			break;
		}	
		case Low_Fa_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2703);
			break;
		}	
		case Low_So:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2550);
			break;
		}	
		case Low_So_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2408);
			break;
		}	
		case Low_La:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2272);
			break;
		}
		case Low_La_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2145);
			break;
		}	
		case Low_Ti:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,2024);
			break;
		}
		case Mid_Do:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1910);
			break;
		}	
		case Mid_Do_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1804);
			break;
		}	
		case Mid_Re:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1702);
			break;
		}	
		case Mid_Re_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1607);
			break;
		}	
		case Mid_Mi:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1516);
			break;
		}	
		case Mid_Fa:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1431);
			break;
		}	
		case Mid_Fa_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1351);
			break;
		}	
		case Mid_So:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1275);
			break;
		}	
		case Mid_So_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1204);
			break;
		}
		case Mid_La:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1135);
			break;
		}	
		case Mid_La_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1073);
			break;
		}	
		case Mid_Ti:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,1011);
			break;
		}
		case High_Do:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,955);
			break;
		}	
		case High_Do_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,902);
			break;
		}	
		case High_Re:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,850);
			break;
		}	
		case High_Re_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,804);
			break;
		}	
		case High_Mi:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,757);
			break;
		}	
		case High_Fa:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,715);
			break;
		}	
		case High_Fa_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,676);
			break;
		}	
		case High_So:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,637);
			break;
		}	
		case High_So_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,602);
			break;
		}	
		case High_La:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,567);
			break;
		}	
		case High_La_N:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,536);
			break;
		}	
		case High_Ti:
		{
			__HAL_TIM_SET_AUTORELOAD(&htim12,505);
			break;
		}
	}
}

uint16_t Chinese_Poker[500][2] = 
{
 {High_So,220},{0,20},{High_So,110},{High_La,110},{High_So,220},{High_Mi,220},{High_So,880},{High_Do,220},{0,20},{High_Do,110},{High_Re,110},
 {High_Do,220},{Mid_La,220},{Mid_So,880},{Mid_La,220},{Low_La,110},{Low_So,110},{Mid_La,220},{High_Do,220},{High_Re,220},{High_Do,220},
 {High_Re,220},{High_Mi,220},{High_So,330},{High_La,110},{High_So,220},{High_Mi,220},{High_Re,880},
 
 {High_So,220},{0,20},{High_So,110},{High_La,110},{High_So,220},{High_Mi,220},{High_So,880},{High_Re,220},{0,20},{High_Re,110},{High_Mi,110},
 {High_Re,220},{High_Do,220},{Mid_La,880},{Mid_So,220},{Mid_La,220},{High_Do,220},{High_Re,220},{High_Mi,220},{High_La,220},{High_Mi,220},{High_So,220},
 {High_Re,330},{High_Mi,110},{High_Re,220},{Mid_La,220},{High_Do,880},
 
 {High_La,220},{0,20},{High_La,110},{High_So,110},{High_Mi,220},{High_So,220},{High_La,660},{High_Mi,220},{High_So,220},{0,20},{High_So,110},
 {High_La,110},{High_So,220},{High_Mi,220},{High_So,880},{High_Re,220},{0,20},{High_Re,110},{High_Mi,110},{High_Re,220},{High_Do,220},
 {High_Re,220},{High_Do,220},{Mid_La,220},{Mid_So,220},{High_Re,660},{High_Do,220},{High_Mi,880},
  
 {High_La,220},{0,20},{High_La,110},{High_So,110},{High_Mi,220},{High_So,220},{High_La,660},{High_Mi,220},{High_So,220},{0,20},{High_So,110},
 {High_La,110},{High_So,220},{High_Mi,220},{High_So,880},{High_Re,220},{0,20},{High_Re,110},{High_Mi,110},{High_Re,220},{High_Do,220},
 {High_Re,220},{High_Do,220},{Mid_La,220},{Mid_So,220},{High_Do,1320},	
};

uint16_t Mezase_Pokemon_Master[500][2] = 
{
 {Low_La,125},{0,20},{Low_La,125},{0,20},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},
 {Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,375},{0,20},{Low_La,250},{Mid_Do,250},
 {0,20},{Mid_Do,125},{0,20},{Mid_Do,250},{Low_Ti,375},{0,750},
 
 {Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},
 {Mid_Do,125},{Low_La,250},{0,20},{Low_La,125},{Mid_Do,125},{Low_La,250},{0,20},{Low_La,125},
 {Mid_Mi,250},{0,20},{Mid_Mi,250},{Mid_Re,125},{Mid_Do,250},{Low_Ti,375},{0,750},
 
 {Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},
 {Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},{Mid_Do,125},{Low_La,125},
 {0,20},{Low_La,250},{Mid_Do,250},{0,20},{Mid_Do,125},{Mid_Re,250},{Low_Ti,250},{0,500},

 {Low_La,125},{0,20},{Low_La,125},{Mid_Do,125},{Low_La,375},{Mid_Mi,250},{Mid_Re,250},{Mid_Do,125},{Low_La,375},{0,250},
 {Low_La,125},{0,20},{Low_La,125},{Mid_Do,250},{Low_La,250},{0,500},{Mid_La,250},{Mid_So,250},{Mid_Mi,250},{Mid_Re,250},
 {Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,250},{Mid_Fa_N,250},
 {Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,500},{0,500},
 
 {Mid_Do_N,125},{0,20},{Mid_Do_N,125},{0,20},{Mid_Do_N,250},{0,20},{Mid_Do_N,125},{0,20},{Mid_Do_N,250},{0,20},{Mid_Do_N,375},{0,250},
 {Low_La,125},{0,20},{Low_La,125},{0,20},{Low_La,125},{Mid_Do_N,125},{Low_Ti,250},{Mid_Mi,500},{0,1250},
 
 {Mid_Do_N,125},{0,20},{Mid_Do_N,125},{0,20},{Mid_Do_N,250},{0,20},{Mid_Do_N,125},{0,20},{Mid_Do_N,125},{0,20},{Mid_Do_N,125},{0,20},
 {Mid_Do_N,375},{0,250},{Low_La,125},{0,20},{Low_La,125},{0,20},{Low_La,125},{Mid_Do_N,125},{Low_Ti,500},{0,1250},
 
 {Low_Ti,250},{Low_La,250},{Low_Fa_N,250},{Low_La,250},{Low_Ti,250},{Mid_Do_N,250},{Low_La,500},{0,250},{Low_La,250},{0,20},{Low_La,250},
 {0,20},{Low_La,250},{Mid_Mi,250},{Mid_Do_N,500},{0,250},{Low_La,250},{Mid_Do_N,250},{0,20},{Mid_Do_N,250},{Low_Ti,250},{Low_La,250},{Low_So_N,250},
 {Low_La,250},{Low_Ti,250},{Low_La,250},{Low_Ti,250},{Low_La,250},{Low_So_N,250},{Low_La,250},{Low_Ti,250},{0,250},
 
 {Mid_Do_N,250},{Mid_Re,250},{Mid_Mi,250},{Mid_La,250},{0,20},{Mid_La,250},{Mid_Ti,250},{Mid_So_N,250},{Mid_Fa_N,250},{0,20},{Mid_Fa_N,250},
 {Mid_Mi,250},{Mid_Fa_N,250},{Mid_Mi,250},{0,20},{Mid_Mi,250},{Mid_Do_N,250},{Low_La,250},{0,250},
 
 {Low_La,250},{Low_Ti,250},{Mid_Do_N,250},{0,20},{Mid_Do_N,250},{0,20},{Mid_Do_N,250},{Mid_Re,250},{Mid_Mi,250},{0,20},{Mid_Mi,250},{Mid_Re,250},
 {Mid_Do_N,250},{0,20},{Mid_Do_N,250},{Low_Ti,500},{0,750},
 
 {Mid_Do_N,250},{Mid_Re,250},{Mid_Mi,250},{Mid_La,250},{0,20},{Mid_La,250},{Mid_Ti,250},{Mid_So_N,250},{Mid_Fa_N,250},{Mid_Fa_N,250},{Mid_Mi,250},
 {Mid_Fa_N,250},{Mid_Mi,250},{0,20},{Mid_Mi,250},{Mid_Do_N,250},{Low_La,250},{0,20},{Low_La,250},{Mid_Do_N,250},{Low_Ti,250},{Low_La,500},
 {Low_Ti,250},{Mid_Do_N,250},{Low_Ti,500},{Low_La,500},{0,20},{Mid_La,3000},
};

uint16_t Undertale_Megalovania[500][2] = 
{
 {Mid_Re,125},{0,20},{Mid_Re,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Mid_Re,125},{0,20},{Mid_Re,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Low_Ti,125},{0,20},{Low_Ti,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Low_Ti,125},{0,20},{Low_Ti,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 
 {Mid_Re,125},{0,20},{Mid_Re,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Mid_Re,125},{0,20},{Mid_Re,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Low_Ti,125},{0,20},{Low_Ti,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 {Low_Ti,125},{0,20},{Low_Ti,125},{High_Re,250},{Mid_La,375},{Mid_So_N,125},{0,125},{Mid_So,250},{Mid_Fa,250},
 {Mid_Re,125},{Mid_Fa,125},{Mid_So,125},
 
 {Mid_Fa,250},{0,20},{Mid_Fa,125},{0,20},{Mid_Fa,125},{0,125},{Mid_Fa,250},{0,20},{Mid_Fa,250},{Mid_Re,250},
 {0,20},{Mid_Re,625},
 
 {Mid_Fa,250},{0,20},{Mid_Fa,125},{0,20},{Mid_Fa,125},{0,125},{Mid_So,250},{0,20},{Mid_So_N,250},{0,20},
 {Mid_So,63},{Mid_So_N,63},{Mid_So,63},{Mid_Fa,125},{Mid_Re,125},{Mid_Fa,125},{Mid_So,125},{0,250},
 
 {Mid_Fa,250},{0,20},{Mid_Fa,125},{0,20},{Mid_Fa,125},{0,125},{Mid_So,250},{0,20},{Mid_So_N,125},{0,125},
 {Mid_La,250},{High_Do,250},{Mid_La,375},{High_Re,250},{0,20},{High_Re,250},{0,20},{High_Re,125},{Mid_La,125},
 {High_Re,125},{High_Do,675}, 
};

uint16_t PVZ_Grasswalk_1[500][2] = 
{
 {Low_La,500},{Mid_Re_N,250},{Mid_Mi,250},{Mid_La,500},{Mid_Mi,500},{Low_La,500},{Mid_Re_N,250},{Mid_Mi,250},
 {Mid_So,250},{Mid_Fa_N,250},{Mid_Fa,250},{Mid_Mi,250},{Mid_La,250},{Mid_So_N,250},{Mid_Mi,250},{Mid_So,250},
 {Mid_Fa_N,250},{Mid_Re,250},{Mid_Fa,250},{Mid_Mi,250},{Mid_Do,1000},{Low_Fa,250},{Low_Fa_N,250},{Low_So,250},{Low_So_N,250},
 
 {Low_La,500},{Mid_Re_N,250},{Mid_Mi,250},{Mid_La,500},{Mid_Mi,500},{Low_La,500},{Mid_Re_N,250},{Mid_Mi,250},
 {Mid_So,250},{Mid_Fa_N,250},{Mid_Fa,250},{Mid_Mi,250},{Mid_La,250},{Mid_So_N,250},{Mid_Mi,250},{Mid_So,250},
 {Mid_Fa_N,250},{Mid_Re,250},{Mid_Fa,250},{Mid_Mi,250},{Mid_Do,1000},{0,250},{Low_Fa,250},{Low_Mi,250},{Low_Re_N,250},{Mid_Re,1000},
 {0,500},{Mid_Mi,250},{Mid_Fa,250},{Mid_Mi,500},{Low_Ti,500},{Mid_Mi,500},{Mid_Re,500},{Mid_Do,1000},{Mid_Mi,1000},{Mid_La,1500},{0,500},
 
 {Mid_La,250},{Mid_So_N,250},{Mid_La,250},{Mid_Ti,250},{High_Do,500},{0,500},{Mid_La,250},{Mid_So_N,250},{Mid_La,250},{Mid_Ti,250},
 {High_Do,500},{High_Re,500},{Mid_Ti,1000},{High_Mi,1000},{High_So_N,1000},{High_Ti,1000},
 
 {Mid_La,750},{Mid_Ti,250},{High_Do,250},{Mid_Mi,250},{High_Do,250},{Mid_Ti,250},{Mid_La,750},{Mid_Ti,250},{High_Do,250},{High_Re,250},
 {High_Do,250},{Mid_Ti,250},{Mid_La,750},{Mid_Ti,250},{High_Do,250},{Mid_Mi,250},{High_Do,250},{Mid_Ti,250},{Mid_La,1000},{Mid_Ti,1000},
	 
 {Mid_La,750},{Mid_Ti,250},{High_Do,250},{Mid_Mi,250},{High_Do,250},{Mid_Ti,250},{Mid_La,750},{Mid_Ti,250},{High_Do,250},{High_Re,250},
 {High_Do,250},{Mid_Ti,250},{Mid_La,750},{Mid_Ti,250},{High_Do,250},{Mid_Mi,250},{High_Do,250},{Mid_Ti,250},{Mid_La,1000},{Mid_So,1000},{Mid_La,500},
};

uint16_t PVZ_Grasswalk_2[500][2] = 
{
 {Mid_Mi,500},{Mid_La,250},{High_Do,250},{High_Mi,500},{High_Fa,500},{High_Mi,500},{0,20},{High_Mi,250},{High_Do,250},{Mid_La,500},{0,500},
 {High_La,250},{High_So_N,250},{High_Mi,500},{High_So,250},{High_Mi,250},{High_Do,500},{High_Re,250},{Mid_La,250},{High_Re,250},{High_Do,250},{Mid_Ti,1000},
 
 {Mid_Re_N,250},{Mid_Mi,250},{Mid_La,500},{Mid_So,500},{Mid_Fa,500},{Mid_Mi,500},{Mid_La,500},{High_Do,500},{High_Re,500},{High_Mi,500},{High_Do,250},
 {Mid_La,250},{Mid_Ti,500},{High_Do,500},{Mid_La,500},
};

uint16_t PVZ_Graze_The_Roof[100][2] = 
{
 {Low_La,240},{Mid_Mi,120},{0,20},{Mid_Mi,120},{Mid_La,240},{Mid_Mi,240},{0,240},{Mid_Mi,240},{0,240},{Mid_Mi,240},
 {Low_La,240},{Mid_Mi,120},{0,20},{Mid_Mi,120},{Mid_La,240},{Mid_Mi,240},{0,240},{Mid_Mi,240},{0,50},
	
 {High_Re_N,240},{High_Mi,240},{High_La,480},{High_Re_N,240},{High_Mi,240},{Mid_La,480},{High_Re,240},
 {High_Mi,240},{High_So,240},{High_Re,240},{High_Mi,240},{High_So,240},{High_Mi,480},{High_So,240},
 {High_Fa,240},{High_Re,240},{High_Mi,240},{High_Re,240},{High_Do,240},{Mid_La,480},{Mid_So,240},
 {Mid_La,240},{High_Do,240},{High_Re,240},{High_Mi,240},{High_Re,240},{High_Do,480},
 
 {High_Re_N,240},{High_Mi,240},{High_La,480},{High_Re_N,240},{High_Mi,240},{Mid_La,480},{High_Re,240},
 {High_Mi,240},{High_So,240},{High_Re,240},{High_Mi,240},{High_So,240},{High_Mi,480},{High_So,240},
 {High_Fa,240},{High_Re,240},{High_Mi,240},{High_Re,240},{High_Do,240},{Mid_La,480},{Mid_So,240},
 {Mid_La,240},{High_Do,240},{High_Re,240},{High_Mi,240},{High_Re,240},{High_Do,480},{High_Re,240},{Mid_La,480},
};
