/*
 * encoders.h
 *
 * Created: 2020-08-24 19:16:44
 *  Author: Daniel
 */ 

#include <asf.h>

#ifndef ENCODERS_H_
#define ENCODERS_H_

/*
Encoder pins - left Motor:
CH.A  -----> D26
CH.B  -----> D27

Encoder pins - right Motor:
CH.A  -----> D28
CH.B  -----> D29
*/
#define CHA_PIN_MOTOR_LEFT    IOPORT_CREATE_PIN(PIOD, PIO_PD1)   // digital pin 26
#define CHB_PIN_MOTOR_LEFT    IOPORT_CREATE_PIN(PIOD, PIO_PD2)   // digital pin 27
#define CHA_PIN_MOTOR_RIGHT   IOPORT_CREATE_PIN(PIOD, PIO_PD3)   // digital pin 28
#define CHB_PIN_MOTOR_RIGHT   IOPORT_CREATE_PIN(PIOD, PIO_PD6)    // digital pin 29

/* Declaration */
extern uint32_t chA_motor_left;
extern uint32_t chB_motor_left;
extern uint32_t chA_motor_right;
extern uint32_t chB_motor_right;

void pulseCounter_handler_CHA_MOTOR_LEFT(const uint32_t id, const uint32_t index);
void pulseCounter_handler_CHB_MOTOR_LEFT(const uint32_t id, const uint32_t index);
void pulseCounter_handler_CHA_MOTOR_RIGHT(const uint32_t id, const uint32_t index);
void pulseCounter_handler_CHB_MOTOR_RIGHT(const uint32_t id, const uint32_t index);

void pulseCounter_configInterrupt(void);

#endif /* ENCODERS_H_ */