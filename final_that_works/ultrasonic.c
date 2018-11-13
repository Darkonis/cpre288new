/*
 * ultrasonic.c
 *
 *  Created on: Oct 9, 2018
 *      Author: darrona
 */
/**
 * TIMER3_CTL_R: Enable, Edge Select.
TIMER3_CFG_R: 16-bit mode.
TIMER3_TBMR_R: Capture Mode, Edge-Time Mode, Count up.
TIMER3_TBILR_R: Set upper bound.
TIMER3_IMR_R: Enable capture interrupt.
 */
#include "ultrasonic.h"
#include "timer.h"
#include <math.h>
#include <limits.h>

void ultrasonic_init()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable alternate functions on port b pins 0 and 1
    //GPIO_PORTB_AFSEL_R |= 0X08;

    GPIO_PORTB_PCTL_R |= 0x7<<12;
    //set pin 4 and 5 to digital
    GPIO_PORTB_DEN_R |= BIT3;
    GPIO_PORTB_DIR_R |= BIT3;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;//(0X0C00); // both edges
    TIMER3_CFG_R |= TIMER_CFG_16_BIT;
    //TIMER3_TBMR_R =  0x17; //10111
    //Load initial timer value
    TIMER3_TBMR_R |=  0x17;
    TIMER3_TBILR_R = 0xffffffff;
    //TIMER3_TBPR_R = 0XFF;
   // TIMER3_TBPR_R = 0XF;
    //Clear the timeout flag
    //TIMER3_ICR_R = TIMER_ICR_TBTOCINT;


    TIMER3_IMR_R |= TIMER_IMR_CBEIM;//0X400;
    //TIMER3_MIS_R = 0X400;
    //Clear the interrupt mask register
    //TIMER3_ICR_R = TIMER3_ICR_R|0X400;
    //GPIO_PORTB_AFSEL_R |= 0x08;
    //GPIO_PORTB_PCTL_R |= 0x7<<12;
     NVIC_EN1_R |=0X10;
    //Enable Timer5 A
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
    // TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    //set pin 0 to Rx or input
    //GPIO_PORTB_DIR_R &= ~BIT0;
    //set pin 1 to Tx or output

}
void send_pulse()
{
    // timer_startClock();
    GPIO_PORTB_AFSEL_R &= ~0x08;
    GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
    GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08; // set PB3 to low
    GPIO_PORTB_DIR_R &= ~0x08; // set PB3 as input
    GPIO_PORTB_AFSEL_R |= 0x08;
    //GPIO_PORTB_DEN_R |= BIT3;
     // GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input

    // timer_stopClock();
}
volatile unsigned rising_time = 0;
volatile unsigned falling_time = 0;
volatile int update_flag = 0;
volatile int flag2=0;
 //unsigned long time_diff=0;
unsigned long overflow=0;
unsigned ping_read()
{
    send_pulse();
    //printf("%d\n", falling_time-rising_time);
    //while(!update_flag)
    //{

    //}
    unsigned long time_diff=0;
    if (falling_time < rising_time)
    {
    time_diff=  falling_time+(0xFFFFFFFF-rising_time);
    }
    else
    {
        time_diff=falling_time-rising_time;
    }

    //time_diff-=(16777216*overflow);
   /* if(last_time>current_time)
    {

        last_time=UINT_MAX-last_time;
    }
    */
 //   time_diff = (current_time-last_time);
   // time_diff/=overflow;
    update_flag=0;
    flag2=0;
    double d=time2dist(time_diff);
    if(d<30&&d>10)
    {

         return (unsigned )d;
    }
    return (unsigned )d;//time2dist((current_time-last_time)/pow(10.0,6));

}
double time2dist(unsigned time)
{


    return ((34300.0/16000000)*time)/2;
}
void TIMER3B_Handler(void)
{

    if(flag2==0)
    {
        rising_time=TIMER3_TBR_R;
        flag2=1;
    }
    else if (flag2==1)
    {
        falling_time =  TIMER3_TBR_R;
        update_flag=1;
        flag2=2;
    }

    TIMER3_ICR_R = TIMER_ICR_CBECINT;//TIMER3_ICR_R|0X404;
}



