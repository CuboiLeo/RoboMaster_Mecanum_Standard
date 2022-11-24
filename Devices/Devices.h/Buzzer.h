/**
 * @file Buzzer.c
 * @author Leo Liu
 * @brief header file for buzzer
 * @version 1.0
 * @date 2022/07/25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __BUZZER_H
#define __BUZZER_H

#include "tim.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define Buzzer_Func_GroundInit				\
{																			\
		&Buzzer_Init,											\
				&Buzzer_Robot_Initializing,		\
				&Buzzer_Robot_Is_Initialized,	\
				&Buzzer_Modules_Offline,			\
}

#define MAX_VOLUME 270 //Maximum volume

typedef struct
{
	enum
	{
		Low_Do,
		Low_Do_N,
		Low_Re,
		Low_Re_N,
		Low_Mi,
		Low_Fa,
		Low_Fa_N,
		Low_So,
		Low_So_N,
		Low_La,
		Low_La_N,
		Low_Ti,
		
		Mid_Do,
		Mid_Do_N,
		Mid_Re,
		Mid_Re_N,
		Mid_Mi,
		Mid_Fa,
		Mid_Fa_N,
		Mid_So,
		Mid_So_N,
		Mid_La,
		Mid_La_N,
		Mid_Ti,
		
		High_Do,
		High_Do_N,
		High_Re,
		High_Re_N,
		High_Mi,
		High_Fa,
		High_Fa_N,
		High_So,
		High_So_N,
		High_La,
		High_La_N,
		High_Ti,
	}Note;
	
	
}Buzzer_t;

typedef struct
{
	void (*Buzzer_Init)(void);
	void (*Buzzer_Robot_Initializing)(void);
	void (*Buzzer_Robot_Is_Initialized)(void);
	void (*Buzzer_Modules_Offline)(void);
}Buzzer_Func_t;

extern Buzzer_t Buzzer;
extern Buzzer_Func_t Buzzer_Func;

#endif
