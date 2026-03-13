// Nathan Hinton
// Driver code for the motor.

#include "robot.h"
#include "motor.h"

// Initialize the robot instance
void Robot_Init(Robot_t* robot, Motor_t* motor_left, Motor_t* motor_right) {
  robot->motor_left = *motor_left;
  robot->motor_right = *motor_right;

  // Start with the robot stopped
  Robot_Stop(robot);
}

// Drive the robot forward
void Robot_Forward(Robot_t* robot, uint8_t speed, int8_t direction) {
  if ((speed + direction) > 255) {
    Motor_Forward(&robot->motor_left, 255);
  } else {
    Motor_Forward(&robot->motor_left, speed + direction);
  }
  if ((speed - direction) < 0) {
    Motor_Forward(&robot->motor_right, 0);
  } else {
    Motor_Forward(&robot->motor_right, speed - direction);
  }
}

// Drive the robot forward
void Robot_Backward(Robot_t* robot, uint8_t speed, int8_t direction) {
  if ((speed + direction) > 255) {
    Motor_Backward(&robot->motor_left, 255);
  } else {
    Motor_Backward(&robot->motor_left, speed + direction);
  }
  if ((speed - direction) < 0) {
    Motor_Backward(&robot->motor_right, 0);
  } else {
    Motor_Backward(&robot->motor_right, speed - direction);
  }
}

// Turn the robot on a point
void Robot_Spin(Robot_t* robot, int16_t speed) {
  if (speed < -255) {
    speed = -255;
  } else if (speed > 255) {
    speed = 255;
  }
  if (speed < 0) { // Spin counter clockwise
    Motor_Backward(&robot->motor_left, speed);
    Motor_Forward(&robot->motor_right, speed);

  } else {
    Motor_Forward(&robot->motor_left, speed);
    Motor_Backward(&robot->motor_right, speed);
  }
}

// Stop the robot
void Robot_Stop(Robot_t* robot) {
  Motor_Stop(&robot->motor_left);
  Motor_Stop(&robot->motor_right);
}
