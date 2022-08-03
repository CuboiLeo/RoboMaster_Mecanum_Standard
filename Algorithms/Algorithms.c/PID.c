/**
 * @file PID.c
 * @author Leo Liu
 * @brief PID Algorithms
 * @version 1.0
 * @date 2022-07-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include "PID.h"

float Positional_PID(PID_t *PID, float Target_Value, float Measured_Value);
float Incremental_PID(PID_t *PID, float Target_Value, float Measured_Value);
void Clear_PID_Data(PID_t *PID);

//Initializing all the used PID parameters in the header file
PID_t Chassis_Angle_PID = Chassis_Angle_PIDInit;
#undef Chassis_Angle_PIDInit

PID_t Chassis_Speed_PID = Chassis_Speed_PIDInit;
#undef Chassis_Angle_PIDInit

PID_t Fric_Wheel_PID = Fric_Wheel_PIDInit;
#undef Fric_Wheel_PIDInit

PID_t Yaw_Angle_PID = Yaw_Angle_PIDInit;
#undef Yaw_Angle_PIDInit

PID_t Yaw_Speed_PID = Yaw_Speed_PIDInit;
#undef Yaw_Speed_PIDInit

PID_t Pitch_Angle_PID = Pitch_Angle_PIDInit;
#undef Pitch_Angle_PIDInit

PID_t Pitch_Speed_PID = Pitch_Speed_PIDInit;
#undef Pitch_Speed_PIDInit

PID_t Trigger_Angle_PID = Trigger_Angle_PIDInit;
#undef Trigger_Angle_PIDInit

PID_t Trigger_Speed_PID = Trigger_Speed_PIDInit;
#undef Trigger_Speed_PIDInit

PID_Func_t PID_Func = PID_Func_GroundInit;
#undef PID_Func_GroundInit

float Positional_PID(PID_t *PID, float Target_Value, float Measured_Value)
{
	PID->Target_Value = Target_Value;
	PID->Measured_Value = Measured_Value;
	PID->Prev_Error = PID->Error;
	PID->Error = PID->Target_Value - PID->Measured_Value;
	
	PID->P_Out = PID->Kp * PID->Error;
	PID->I_Out += PID->Ki * PID->Error;
	PID->D_Out = PID->Kd * (PID->Error - PID->Prev_Error);
	
	PID->I_Out = VAL_LIMIT(PID->I_Out,PID->I_Out_Max,-PID->I_Out_Max);
	PID->Output = (PID->P_Out + PID->I_Out + PID->D_Out);
	PID->Output = VAL_LIMIT(PID->Output,PID->Output_Max,-PID->Output_Max);
	
	return PID->Output;
}

float Incremental_PID(PID_t *PID, float Target_Value, float Measured_Value)
{
	PID->Target_Value = Target_Value;
	PID->Measured_Value = Measured_Value;
	PID->Prev_Prev_Error = PID->Prev_Error;
	PID->Prev_Error = PID->Error;
	PID->Error = PID->Target_Value - PID->Measured_Value;
	
	PID->P_Out = PID->Kp * (PID->Error - PID->Prev_Error);
	PID->I_Out = PID->Ki * PID->Error;
	PID->D_Out = PID->Kd * (PID->Error - 2.0f * PID->Prev_Error + PID->Prev_Prev_Error);
	
	PID->I_Out = VAL_LIMIT(PID->I_Out,PID->I_Out_Max,-PID->I_Out_Max);
	PID->Output = (PID->P_Out + PID->I_Out + PID->D_Out);
	PID->Output = VAL_LIMIT(PID->Output,PID->Output_Max,-PID->I_Out_Max);
	
	return PID->Output;
}

void Clear_PID_Data(PID_t *PID)
{
	PID->Target_Value = 0;
	PID->Measured_Value = 0;
	PID->Prev_Prev_Error = 0;
	PID->Prev_Error = 0;
	PID->Error = 0;
	PID->P_Out = 0;
	PID->I_Out = 0;
	PID->D_Out = 0;
	PID->Output = 0;
}
