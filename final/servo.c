/*
 * servo.c
 *
 *  Created on: Oct 17, 2018
 *      Author: darrona
 */
#include "servo.h"
#define BIT6 (*((volatile unsigned long *) 0x07))
#define BIT5 (*((volatile unsigned long *) 0x06))
void servo_init()
{
    //unsigned pulse_period = 344000;
    unsigned pulse_period = 320000;
    unsigned mid_width=24000;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
   // SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
          //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
         // SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
          //enable alternate functions on port b5
          GPIO_PORTB_AFSEL_R = 0x20;
          //enable on pin5
          GPIO_PORTB_PCTL_R = 0x7<<20;
          //set pin 4 and 5 to digital
          GPIO_PORTB_DEN_R =  0x20;
          //set pin 0 to Rx or input
          //GPIO_PORTB_DIR_R &= ~BIT6;
          //set pin 1 to Tx or output
         GPIO_PORTB_DIR_R =  0x20;
    //***set GPIO PB5, turn on clk, alt. function, output, enable***
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    TIMER1_CTL_R &= ~TIMER_CTL_TBEN;
    //TIMER1_TBCMR_R=0;
    //SYSCTL_RCGCTIMER_R = _____ //turn on clk for timer0
    TIMER1_TBMR_R |= 0xA; //periodic and PWM enable
    TIMER1_CFG_R = 0x4; //set size of timer to 16
    TIMER1_TBILR_R = pulse_period & 0xFFFF; //lower 16 bits of the interval
    TIMER1_TBPR_R = pulse_period >> 16; //set the upper 8 bits of the interval
    //int i=pulse_period - mid_width;
    TIMER1_TBMATCHR_R = (pulse_period - mid_width)&0xffff; // if you want to move servo to the middle
    TIMER1_TBPMR_R=(pulse_period - mid_width)>>16;
    TIMER1_CTL_R = TIMER_CTL_TBEN; //enable timer

}
//wifipassword cpre288psk
/*//cbot 7
int max=37500;
int min=5000;
*/
//cybot 13 DO NOT USE THE IR IS WRONG
/*
int max=37500;
int min=10000;
*/
//cybot 9
/*
int max=37500;
int min=5000;
*/
//cybot 11
int max=37500;
int min=9000;
void calibrate()
{
    lcd_init();
    button_init();
    int done=0;
    int part=0;

    unsigned pulse_period = 320000;
    unsigned val=24000;
    while(!done)
    {
        int btn =button_getButton();
        if(btn==6&&val!=100000)
        {
            val+=1000;
        }
        if(btn ==1&&val!=0)
        {
            val-=1000;
        }
        if(btn==5&&val!=100000)
                {
                    val+=500;
                }
                if(btn ==2&&val!=0)
                {
                    val-=500;
                }
        if(btn==4||btn==3)
        {
            if(part==0)
            {
                max=val;
            }
            else
            {
                min=val;
            }
            part++;
            timer_waitMillis(300);
        }
        if(part==2)
        {
            done=1;
            continue;
        }
        if(part==0)
        {

            lcd_printf("move to full left. :cur val %u",val);
        }
        else
        {
            lcd_printf("move to full right. :cur val %u",val);
        }
        unsigned pulse_period = 320000;
        timer_waitMillis(100);

             TIMER1_TBMATCHR_R = (pulse_period - val)&0xffff; // if you want to move servo to the middle
                 TIMER1_TBPMR_R=(pulse_period - val)>>16;
    }

}
void move_manual()
{
    float degree=1500;
    float degreeinc=1500/90.0;
    int direction =1;
    int pressed=0;

    while(1)
    {
        float toPrint=((degree-1000)/1000)*180;
        int btn =button_getButton();
        if(direction==-1)
        {
        lcd_printf("current degree=%.2f direction =decrement",toPrint);
        }
        if(direction==1)
        {
                lcd_printf("current degree=%.2f direction =increment",(toPrint));
        }

        if(btn==1)
        {
            degree+=direction*degreeinc;
        }
        if(btn==2)
        {
            degree+=direction*2.5*degreeinc;
        }
        if(btn==3)
                {
                    degree+=direction*5*degreeinc;
                }
        if(btn==4)
                {
                    direction*=-1;
                }

        if(degree<0)
        {
            degree=0;
        }
        else if(degree>20000)
        {
            degree=2000;
        }

        timer_waitMillis(200);
        move_dumb_servo((unsigned)degree*16);

    }
}
void move_dumb_servo(unsigned degree)
{
    unsigned pulse_period = 320000;
       // lcd_printf("move to full right. :cur val %u",tar);

         TIMER1_TBMATCHR_R = (pulse_period - degree)&0xffff; // if you want to move servo to the middle
             TIMER1_TBPMR_R=(pulse_period - degree)>>16;
}
void move_servo(unsigned degree)
{
    float f =(degree/180.0)*(max-min) +min;
    unsigned tar =f;
    unsigned pulse_period = 320000;
   // lcd_printf("move to full right. :cur val %u",tar);

     TIMER1_TBMATCHR_R = (pulse_period - tar)&0xffff; // if you want to move servo to the middle
         TIMER1_TBPMR_R=(pulse_period - tar)>>16;

}




