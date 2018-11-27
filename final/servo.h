/*
 * servo.h
 *
 *  Created on: Oct 17, 2018
 *      Author: darrona
 */

#ifndef SERVO_H_
#define SERVO_H_
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "button.h"
#include "lcd.h"
void servo_init(void);
void move_servo(unsigned degree);
void calibrate();
void move_manual();



#endif /* SERVO_H_ */
