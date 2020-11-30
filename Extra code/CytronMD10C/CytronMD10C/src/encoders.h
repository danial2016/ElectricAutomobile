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
CH.A  -----> D22
CH.B  -----> D23

Encoder pins - right Motor:
CH.A  -----> D24
CH.B  -----> D25
*/
#define CHA_PIN_MOTOR_LEFT    IOPORT_CREATE_PIN(PIOB, PIO_PB26)   // digital pin 22
#define CHB_PIN_MOTOR_LEFT    IOPORT_CREATE_PIN(PIOA, PIO_PA14)   // digital pin 23
#define CHA_PIN_MOTOR_RIGHT   IOPORT_CREATE_PIN(PIOA, PIO_PA15)   // digital pin 24
#define CHB_PIN_MOTOR_RIGHT   IOPORT_CREATE_PIN(PIOD, PIO_PD0)    // digital pin 25

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