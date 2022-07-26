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

#define MAX_VOLUME 270

typedef struct
{
	enum
	{
		Low_Do,
		Low_Re,
		Low_Mi,
		Low_Fa,
		Low_So,
		Low_La,
		Low_Ti,
		
		Mid_Do,
		Mid_Re,
		Mid_Mi,
		Mid_Fa,
		Mid_So,
		Mid_La,
		Mid_Ti,
		
		High_Do,
		High_Re,
		High_Mi,
		High_Fa,
		High_So,
		High_La,
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
