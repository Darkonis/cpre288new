/*
*
*   uart.h
*
*   Used to set up the RS232 connector and WIFI module
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*/

#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"

void uart_init(void);

void uart_sendChar(char data);

char uart_receive(void);

void uart_sendStr(const char *data);
void UART1_Handler(void);
void  ADC0SS0_Handler();
int get_dist();

#endif /* UART_H_ */
