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
void Buzzer_Robot_Initializing(void);
void Buzzer_Robot_Is_Initialized(void);
void Buzzer_Modules_Offline(void);

Buzzer_Func_t Buzzer_Func = Buzzer_Func_GroundInit;
#undef Buzzer_Func_GroundInit

void Buzzer_On(uint16_t Volume, uint16_t Duration);
void Buzzer_Choose_Note(uint16_t Note);
void Buzzer_Play_Song(uint16_t Song[],uint16_t Song_Length);

Buzzer_t Buzzer;

//Enable timer's PWM function
void Buzzer_Init(void)
{
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
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

void Buzzer_Play_Song(uint16_t Song[], uint16_t Song_Length)
{
	for(int i = 0; i < Song_Length; i++)
	{
		Buzzer_Choose_Note(Song[i]);
		//Buzzer_On(MAX_VOLUME);
	}
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
