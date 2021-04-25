/*
 * timer_interrupt.h
 *
 * Created: 2021-01-04 13:54:41
 *  Author: Danial Mahmoud
 */ 

 #include <asf.h>

#ifndef TIMER_INTERRUPT_H_
#define TIMER_INTERRUPT_H_

void configure_tc_interrupt(void);
void delayMs(uint32_t ms);

/* This variable can be used for timeouts in polling loops such as when waiting for UART receive message */
extern uint32_t systicks;

#endif /* TIMER_INTERRUPT_H_ */