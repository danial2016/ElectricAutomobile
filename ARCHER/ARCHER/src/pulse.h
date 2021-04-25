/*
 * pulse.h
 *
 * Created: 2019-03-21 15:51:30
 *  Author: Daniel
 */ 

#ifndef PULSE_H_
#define PULSE_H_

void pulse_init(void);
void pulse_start(double duty_cycle);
void pulse_set_duty_cycle_left(double duty_cycle);
void pulse_set_duty_cycle_right(double duty_cycle);
void pulse_stop(void);

#endif /* PULSE_H_ */