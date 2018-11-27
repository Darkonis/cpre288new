#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1
#include "servo.h"
#include "ultrasonic.h"
#include "cyBot_uart.h"
//#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manuel_control();
void turn_clockwise(int power);
void turn_counterclockwise(int power);
//void turn(power);
void turn_angle(int angle,int power, oi_t *sensor_data );
void test_things(oi_t *sensor_data);
int bump_check(oi_t *sensor_data);
int cliff_check(oi_t *sensor_data);
