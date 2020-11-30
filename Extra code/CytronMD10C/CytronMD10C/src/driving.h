/*
 * driving.h
 *
 * Created: 2019-07-07 15:40:34
 *  Author: Daniel
 */ 

#include <asf.h>

#ifndef DRIVING_H_
#define DRIVING_H_

// Motor A
#define PWM_pin_MotorA  PIO_PB25_IDX   // Digital pin 2 
#define DIR_pin_MotorA  PIO_PC28_IDX   // Digital pin 3

// Motor B
#define PWM_pin_MotorB  PIO_PA2_IDX	   // Analog In 7
#define DIR_pin_MotorB  PIO_PC25_IDX   // Digital pin 5

#define FORWARD    1
#define REVERSE    0

void init_motors(void);
void drive_forward(int speed);
void drive_backwards(int speed);
void accelerate(int speed);
void decelerate(int start_speed);
void set_direction(int dir);

#endif /* DRIVING_H_ */