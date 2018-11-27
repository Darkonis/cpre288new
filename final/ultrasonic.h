/*
 * ultrasonic.h
 *
 *  Created on: Oct 9, 2018
 *      Author: darrona
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
void ultrasonic_init();
unsigned ping_read();
double time2dist(unsigned time);
void send_pulse(void);
void TIMER3B_Handler(void);

#endif /* ULTRASONIC_H_ */
