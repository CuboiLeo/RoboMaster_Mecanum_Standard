/**
 * @file CRC_Verification.h
 * @author Leo Liu
 * @brief header file for CRC verification
 * @version 1.0
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __CRC_VERIFICATION_H
#define __CRC_VERIFICATION_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CRC_Verif_Func_GroundInit		\
{																		\
		&Get_CRC8_Check_Sum,						\
				&Verify_CRC8_Check_Sum,			\
				&Append_CRC8_Check_Sum,			\
				&Get_CRC16_Check_Sum,				\
				&Verify_CRC16_Check_Sum,		\
				&Append_CRC16_Check_Sum,		\
}

typedef struct
{
	unsigned char (*Get_CRC8_Check_Sum)(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
	unsigned int (*Verify_CRC8_Check_Sum)(unsigned char *pchMessage, unsigned int dwLength);
	void (*Append_CRC8_Check_Sum)(unsigned char *pchMessage, unsigned int dwLength);
	uint16_t (*Get_CRC16_Check_Sum)(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
	uint32_t (*Verify_CRC16_Check_Sum)(uint8_t *pchMessage, uint32_t dwLength);
	void (*Append_CRC16_Check_Sum)(uint8_t *pchMessage, uint32_t dwLength);
}CRC_Verif_Func_t;

extern CRC_Verif_Func_t CRC_Verif_Func;

#endif
