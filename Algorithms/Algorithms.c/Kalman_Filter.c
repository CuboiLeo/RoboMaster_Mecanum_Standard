/**
 * @file Kalman_Filter.c
 * @author Leo Liu
 * @brief Kalman Filter
 * @version 1.0
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 #include "Kalman_Filter.h"
 
 void Kalman_Filter(float Measured_Angle, float Measured_Gyro, float *Output_Angle, float *Output_Angular_Speed)		
{
    //------------------------------
    static float Angle, Angle_Dot; 		
    static float P[2][2]={
													{ 1, 0 },
													{ 0, 1 }
																	};	
    static float P_Dot[4] = {0, 0, 0, 0};
    static float Q_Bias, Angle_Error, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
    //------------------------------
    Angle += (Measured_Gyro - Q_Bias) * SAMPLE_PERIOD;

    P_Dot[0] = Q_ANGLE - P[0][1] - P[1][0];
    P_Dot[1] = -P[1][1];
    P_Dot[2] = -P[1][1];
    P_Dot[3] = Q_GYRO;

    P[0][0] += P_Dot[0] * SAMPLE_PERIOD;
    P[0][1] += P_Dot[1] * SAMPLE_PERIOD;
    P[1][0] += P_Dot[2] * SAMPLE_PERIOD;
    P[1][1] += P_Dot[3] * SAMPLE_PERIOD;

    Angle_Error = Measured_Angle - Angle;

    PCt_0=C_0 * P[0][0];
    PCt_1=C_0 * P[1][0];

    E = R_ANGLE + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
        
    Angle	+= K_0 * Angle_Error;
    Q_Bias += K_1 * Angle_Error;
    Angle_Dot = Measured_Gyro - Q_Bias;

    *Output_Angle = Angle;
    *Output_Angular_Speed = Angle_Dot;
}
