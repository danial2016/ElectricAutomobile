/*
 * test_cases.c
 *
 * DESCRIPTION MISSING!
 *
 * Created: 2020-08-10 18:42:23
 *  Author: Daniel
 */ 

#include "test_cases.h"
#include "driving.h"

void test_case_1(int target_speed)
{
	set_direction(FORWARD);
	accelerate(target_speed);
	drive_forward(target_speed);
}

void test_case_2(int target_speed)
{
	set_direction(REVERSE);
	accelerate(target_speed);
	drive_backwards(target_speed);
}

void test_case_3(int target_speed)
{
	set_direction(FORWARD);
	accelerate(target_speed);
	drive_forward(target_speed);
	delay_ms(5000);
	int start_speed = target_speed;
	decelerate(start_speed);
	set_direction(REVERSE);
	accelerate(target_speed);
	drive_backwards(50);
	delay_ms(5000);
	decelerate(start_speed);
	delay_ms(1000);
}

void test_case_4(int target_speed)
{
	set_direction(FORWARD);
	accelerate(target_speed);
	drive_forward(target_speed);
	delay_ms(10000);
	delay_ms(5000);
	int start_speed = target_speed;
	decelerate(start_speed);
}