/*
 * test_cases.h
 *
 * A test suite for testing motor driving functionality. This includes basic test drives
 * such as driving forward or reverse to a bit more complicated combinations. All drives
 * have an acceleration and/or deceleration phase. This is necessary to avoid gear tear.
 * The description to what each test case does is provided in the function declaration
 * contained in this h-file and implemented in the function definition in the source-file. 
 * 
 * Some notes:
 * - Direction FORWARD and REVERSE is defined from the robot's frame of reference
 * 
 * Created: 2020-08-10 18:42:40
 *  Author: Daniel
 */ 


#ifndef TEST_CASES_H_
#define TEST_CASES_H_

/* Drive forward with velocity at target speed continuously at constant speed */
void test_case_1(int target_speed);
/* Drive backwards with velocity at target speed continuously at constant speed */
void test_case_2(int target_speed);
/* Drive forward for 5 sec, reverse and then drive backwards for 5 sec */
void test_case_3(int target_speed);
/* Drive forward for 19 seconds (length of test course), then stop. This test case is a design program for
   an experiment that is meant to determine whether a PID-controller is needed or not. */
void test_case_4(int target_speed);

#endif /* TEST_CASES_H_ */