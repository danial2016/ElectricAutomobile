/*
 * driving.c
 *
 * Created: 2019-07-07 15:39:56
 *  Author: Daniel
 */ 

#include "driving.h"
#include "pulse.h"
#include <asf.h>

static void setup_direction_pin(void);
static void maintain_speed(int speed);

void init_motors(void)
{
	setup_direction_pin();
	pulse_init();
	pulse_start(0);
}

/*
void drive_forward(int speed)
{
	maintain_speed(speed);
}

void drive_backwards(int speed)
{
	maintain_speed(speed);
}*/

// Sets motor direction, forward or left
void set_direction(int dir)
{
	if(dir == FORWARD)
	{
		// don't remember why they're opposites ...
		ioport_set_pin_level(DIR_pin_MotorA, 1);
		ioport_set_pin_level(DIR_pin_MotorB, 0);
	}
	if(dir == REVERSE)
	{
		ioport_set_pin_level(DIR_pin_MotorA, 0);
		ioport_set_pin_level(DIR_pin_MotorB, 1);
	}
}

/*
void accelerate(int speed)
{
	for (int i = 0; i < speed; i++)
	{
		pulse_set_duty_cycle(i);
		pulse_set_duty_cycle(i);
		delay_ms(50);
	}
}

void decelerate(int start_speed)
{
	for (int i = start_speed; i >= 0; i--)
	{
		pulse_set_duty_cycle(i);
		pulse_set_duty_cycle(i);
		delay_ms(50);
	}
}*/

static void setup_direction_pin(void)
{
	// All motor control pins are outputs
	ioport_init();
	
	// Initialize direction pin for motor A and B
	ioport_set_pin_dir(DIR_pin_MotorA, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(DIR_pin_MotorB, IOPORT_DIR_OUTPUT);
}

/*
static void maintain_speed(int speed)
{
	pulse_set_duty_cycle(speed);
}*/


