/**
 * @file User_Defined_Math.h
 * @author Leo Liu
 * @brief Basic Math/Logic Operations
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define DEG_TO_RAD(DEG) (DEG * 0.0174532925199432957692369076848f)
#define RAD_TO_DEG(RAD)	(RAD * 57.295779513082320876798154814105f)
#define ANGLE_LIMIT_360(val, angle) \
  do                                \
  {                                 \
    (val) = (angle) - (int)(angle); \
    (val) += (int)(angle) % 360;    \
  } while (0)

#define ANGLE_LIMIT_180(val, angle) \
  do                                \
  {                                 \
    (val) = (angle) - (int)(angle); \
    (val) += (int)(angle) % 360;    \
    if((val)>180)                   \
      (val) -= 360;                 \
  } while (0)

#define VAL_MIN(a, b) ((a) < (b) ? (b) : (a))
#define VAL_MAX(a, b) ((a) > (b) ? (b) : (a))
 
extern float inv_sqrt(float x);
extern float VAL_LIMIT(float Value, float Limit);
