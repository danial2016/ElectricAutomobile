/*
 * driving.c
 *
 * Created: 2019-07-07 15:39:56
 *  Author: Daniel
 */ 

#include "driving.h"
#include "pulse.h"

void setup_motor_pins() 
{
	// All motor control pins are outputs
	ioport_init();
	
	// Initialize motor pins for Motor A
	ioport_set_pin_dir(EnA, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(In1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(In2, IOPORT_DIR_OUTPUT);
	
	// Initialize motor pins for Motor B
	ioport_set_pin_dir(EnB, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(In3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(In4, IOPORT_DIR_OUTPUT);
}

// Extra function similar to Arduino IDE's functions, just so that the driving-program
// becomes easier to write since we are copying the Arduino sketch.
void analogWrite(int pin, double duty_cycle)
{
	if(pin == EnA)
	{
		pulse_set_duty_cycle(EnA, duty_cycle);
	}
	if (pin == EnB)
	{
		pulse_set_duty_cycle(EnB, duty_cycle);
	}
}

void decelarate(int end_speed)
{
	for(int i = 100; i >= end_speed; i--)
	{
		analogWrite(EnA, i); // 'i' equals duty_cycle
		analogWrite(EnB, i);
		delay_ms(200);
	}
}

void accelerate(int start_speed)
{
	// turn on motors A and B
	ioport_set_pin_level(In1, HIGH);
	ioport_set_pin_level(In2, LOW);
	ioport_set_pin_level(In4, HIGH);
	ioport_set_pin_level(In2, LOW);
	pulse_start(EnA, start_speed);
	pulse_start(EnB, start_speed);
	
	//increase speed (1000/delay_ms) units per second 
	for(int i = start_speed; i <= 100; i++)
	{
		analogWrite(EnA, i);
		analogWrite(EnB, i);
		delay_ms(200);
	}
}

void drive_forward(double speed) 
{
	// turn on motor A
	ioport_set_pin_level(In1, HIGH);
	ioport_set_pin_level(In2, LOW);
	// turn on motor B
	ioport_set_pin_level(In3, HIGH);
	ioport_set_pin_level(In4, LOW);
	// kick off with initial speed 
	pulse_start(EnA, speed);
	pulse_start(EnB, speed);
	//analogWrite(EnA, speed); vf ANALOG_WRITE????	
}

void reverse()
{
	// Reverse Motor A
	ioport_set_pin_level(In1, LOW);
	ioport_set_pin_level(In2, HIGH);
	// Reverse Motor B
	ioport_set_pin_level(In3, LOW);
	ioport_set_pin_level(In4, HIGH);
}

void turn_off_motors()
{
	ioport_set_pin_level(In1, LOW);
	ioport_set_pin_level(In2, LOW);
	ioport_set_pin_level(In3, LOW);
	ioport_set_pin_level(In4, LOW);
}

