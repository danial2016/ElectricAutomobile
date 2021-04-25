/*
 * control.h
 *
 * Created: 2021-04-25 11:11:20
 *  Author: Danial Mahmoud
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_

/* Maintains a constant robot velocity at the target speed
 * given in encoder values per Ts
 */
void control_speed(uint16_t target_speed);


#endif /* CONTROL_H_ */