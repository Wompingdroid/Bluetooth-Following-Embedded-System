/*
 * motor.h
 *
 *  Created on: Mar 5, 2026
 *      Author: Nathan Hinton
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32l4xx_hal.h"

typedef struct {
  TIM_HandleTypeDef* htim;
  uint32_t channel;
  GPIO_TypeDef* dirPort;
  uint16_t dirPin;
} Motor_t;

// Initialize motor instance
void Motor_Init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, GPIO_TypeDef* dirPort, uint16_t dirPin);

// Control motor
void Motor_Forward(Motor_t* motor, uint8_t speed);
void Motor_Backward(Motor_t* motor, uint8_t speed);
void Motor_Stop(Motor_t* motor);

#endif /* INC_MOTOR_H_ */
