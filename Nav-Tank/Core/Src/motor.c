/*
 * motor.c
 *
 *  Created on: Mar 5, 2026
 *      Author: nathan
 */

#include "motor.h"

void Motor_Init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel, GPIO_TypeDef* dirPort, uint16_t dirPin) {
  motor->htim = htim;
  motor->channel = channel;
  motor->dirPort = dirPort;
  motor->dirPin = dirPin;

  // Start PWM
  HAL_TIM_PWM_Start(motor->htim, motor->channel);

  // Stop motor initially
  Motor_Stop(motor);
}

void Motor_Forward(Motor_t* motor, uint8_t speed) {
  HAL_GPIO_WritePin(motor->dirPort, motor->dirPin, GPIO_PIN_SET);
  __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, speed);
}

void Motor_Backward(Motor_t* motor, uint8_t speed) {
  HAL_GPIO_WritePin(motor->dirPort, motor->dirPin, GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, speed);
}

void Motor_Stop(Motor_t* motor) {
  __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, 0);
}
