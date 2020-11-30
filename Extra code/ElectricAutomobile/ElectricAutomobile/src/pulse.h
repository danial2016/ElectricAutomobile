/*
 * pulse.h
 *
 * Created: 2019-03-21 15:51:30
 *  Author: Daniel
 */ 

#ifndef PULSE_H_
#define PULSE_H_

#define TC_WAVE_CHANNEL_0     0 // PWM channel for Motor A
#define TC_WAVE_CHANNEL_1     1 // PWM channel for Motor B

void pulse_start(int pin, double duty_cycle);
void pulse_set_duty_cycle(int pin, double duty_cycle);
void pulse_stop(void);

#endif /* PULSE_H_ */