[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/CuboiLeo/Robomaster_Mecanum_Infantry_2022/pulse) 
[![Maintainer](https://img.shields.io/badge/Maintainer-Leo-blue)](https://github.com/CuboiLeo)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

# Purdue Robomaster Infantry 2022
- Written using STM32 HAL Library and CubeMX.
- Main Control Board: DJI Robomaster Board A(STM32F427IIH6)

## Basic Components Include:
- 4 * M3508 Motors for Chassis
- 2 * M3508 Motors for Friction Wheels
- 2 * GM6020 Motors for Gimbal (Yaw & Pitch)
- 1 * M2006 Motor for Trigger
- 1 * Super Capacitor for Temporary Power Overload

## Current Function & Future Improvements
- Communications
  - [x] DR16 Remote Control
  - [x] Board A IMU
  - [x] MPU6050 IMU
  - [x] Motors
  - [x] Super Capacitor
  - [ ] Referee System

- Basic Controls
  - [x] Chassis (Forward/Backward, Leftward/Rightward, Rotate)
  - [x] Gimbal (Yaw, Pitch)
  - [x] Shooting (Friction Wheel On/Off, Single Shot, Burst)
  
- Advance Controls
  - [x] Follow Gimbal Mode (Chassis Follow Gimbal)
  - [x] Not Follow Gimbal Mode (Chassis Not Follow Gimbal)
  - [x] SpinTop Mode (Gimbal Holds in Position while Chassis Spins)
  - [ ] Power Limiting (Requires Referee System Communication)
  - [ ] Shooter Heat Regulation (Requires Referee System Communication)
  - [ ] Level Up Adjustments (Requires Referee System Communication)

## System Flow Chart
```mermaid
graph TD;
    BoilerBot_Infantry_2022-->Interrupt_Timer;
    Interrupt_Timer--USART1-->DR16_Remote_Control;
    Interrupt_Timer--USART6-->Referee_System;
    BoilerBot_Infantry_2022-->FreeRTOS;
    FreeRTOS-->General_Initialization;
    FreeRTOS-->IMU_Communication_&_Calculation;
    IMU_Communication_&_Calculation--SPI5-->MPU6500;
    IMU_Communication_&_Calculation--I2C-->MPU6050;
    FreeRTOS-->CAN_Send_Message;
    CAN_Send_Message-->Send_to_All;
    FreeRTOS-->CAN1_Receive_Message;
    CAN1_Receive_Message-->M2006_Trigger_Motor;
    CAN1_Receive_Message-->Super_Capacitor;
    CAN1_Receive_Message-->4*M3508_Chassis_Motors;
    CAN1_Receive_Message-->GM6020_Yaw_Motor;
    FreeRTOS-->CAN2_Receive_Message;
    CAN2_Receive_Message-->2*M3508_Friction_Wheel_Motors;
    CAN2_Receive_Message-->GM6020_Pitch_Motor;
    FreeRTOS-->Robot_Control;
    Robot_Control-->State_Machine;
    Robot_Control-->Chassis_Control;
    Robot_Control-->Gimbal_Control;
    Robot_Control-->Shooting_Control;
```
