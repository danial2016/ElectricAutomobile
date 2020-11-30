/*
 * led.h
 *
 * Created: 2018-03-21 12:04:41
 *  Author: Daniel
 */ 

#include <inttypes.h>

#ifndef LED_H_
#define LED_H_

void disable_write_protect(void);
void enable_pin(void);
void set_output(void);
void clear_output(void);


#endif /* LED_H_ */