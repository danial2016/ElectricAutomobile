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
#define EnA  PIO_PB25_IDX   // Digital pin 2
#define In1  PIO_PB26_IDX   // Digital pin 22
#define In2  PIO_PA14_IDX   // Digital pin 23

// Motor B
#define EnB  PIO_PC28_IDX   // Digital pin 3
#define In3  PIO_PA15_IDX   // Digital pin 24
#define In4  PIO_PD0_IDX    // Digital pin 25

void setup_motor_pins(void);
void analogWrite(int pin, double duty_cycle);
void accelerate(int start_speed);
void decelarate(int end_speed);
void drive_forward(double speed);
void reverse(void);
void turn_off_motors(void);

#endif /* DRIVING_H_ */