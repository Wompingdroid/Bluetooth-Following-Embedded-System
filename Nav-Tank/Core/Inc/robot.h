// Nathan Hinton
// Driver for the robot


#ifndef INC_DRIVER_H_
#define INC_DRIVER_H_

#include "motor.h"
#include "stm32l4xx_hal.h"

typedef struct {
  Motor_t motor_left;
  Motor_t motor_right;
} Robot_t;

// Initialize the robot instance
void Robot_Init(Robot_t* robot, Motor_t* motor_left, Motor_t* motor_right);

// Drive the robot forward
void Robot_Forward(Robot_t* robot, uint8_t speed, int8_t direction);

//Drive the robot backward
void Robot_Backward(Robot_t* robot, uint8_t speed, int8_t direction);

// Turn the robot on a point
void Robot_Spin(Robot_t* robot, int16_t speed);

void Robot_Stop(Robot_t* robot);

#endif
