#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1
#include "servo.h"
#include "ultrasonic.h"
#include "cyBot_uart.h"
#include "movement.h"
//#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void scan();
void turn_angle(int angle,int power, oi_t *sensor_data);
typedef struct thing {
    char width;
    char start_angle;
    char distance;
    char end_angle;
    char dist_1;
    char dist_2;
} thing_t;
int last_dist=0;
//dont use cybot 6!! it dosnt handle
void main() {

    oi_t *sensor_data = oi_alloc();
                       oi_init(sensor_data);

                       oi_setWheels(0,0);//stop
                        //button_init();
           servo_init();
           ultrasonic_init();
           uart_init();
           lcd_init();
            //           scan();
                       turn_angle(90,100,sensor_data);
                       oi_setWheels(0,0);//stop
                       turn_angle(-90,100,sensor_data);
                       oi_setWheels(0,0);//stop
       //calibrate();
while (1)
{
    manuel_control();
                  }
                  oi_setWheels(0, 0); // stop
                  oi_free(sensor_data);
    //lcd_printf("Hello");
   //this calibrates the servo use this if you dont know max and min values
    //lcd_printf("Hello");
        //ultrasonic_init();


    }


void scan()
{

    int i=0;
       // uart_receive();
       // uart_sendStr("test\n");
  // while(1)
        //uart_sendStr("Degrees\tIr distance (cm)\tsonar distance\r\n");

        move_servo(0);
        thing_t found[45];
        int num=-1;
        timer_waitMillis(500);
        //int last_start;
        char msg[100];
        int val=2;
        for(i=0; i<180;i+=val)
            {
            char aout[100];
            char iout[100];
            char sout[100];

            move_servo(i);
            timer_waitMillis(100);
            //strcat(out,"angle ");
            //char t =((char)((int).5*i))+48;
           //fprintf()
            unsigned k = ping_read();
            unsigned l = get_dist();
            sprintf(msg, "Angle:%d\nUltrasonic:%d\nIr:%d\n\0", i,k,l);
            lcd_printf(msg);
            sprintf(msg, "Angle:%d Ultrasonic:%d Ir:%d\n\r\0", i,k,l);
            uart_sendStr(msg);
            sprintf(sout, "%d\t\t\0", k);
            sprintf(iout,"%d\t\t\0", l);
            sprintf(aout,"%d\t\t\0",i);
            //itoa(ping_read(),out,10);
            if(abs(k-l)<6&&k<80)
            {
                val=1;
                if(num==-1)
                {
                    num++;
                    found[num].start_angle=i;
                    found[num].width=0;
                    found[num].distance=((k+l)/2);
                    found[num].end_angle=i;
                    found[num].dist_1=(k+l)/2;
                    found[num].dist_2=found[num].dist_1;

                }
                else if(found[num].end_angle==i-val)//&& abs(((k+l)/2)-last_dist)<5)//if the sensor is still detecting the last object
                {
                //found[num].start_angle=i;
                found[num].width+=val;
                found[num].distance=((found[num].distance+k+l)/3);
                found[num].dist_2=((k+l)/2);
                found[num].end_angle+=val;
                }
                else
                {
                    found[num].end_angle=found[num].width+found[num].start_angle;
                    num= (found[num].width==0)? num:num+1;
                    found[num].start_angle=i;
                    found[num].width=0;
                    found[num].distance=((k+l)/2);
                    found[num].end_angle=i;
                    found[num].dist_1=(k+l)/2;

                }
               last_dist=((k+l)/2);
               sprintf(msg, "Here\n\r\0", i,k,l);
                          uart_sendStr(msg);

            }
            else
            {
                val=2;
            }
            //uart_sendStr(aout);
            //uart_sendStr(iout);
            //uart_sendStr(sout);
            //uart_sendChar('\r');

            }
            //char aout[100];
            //uart_sendStr("\rFound ");
        if(found[num].width==0)
        {
            num--;
        }
        sprintf(msg,"\rFound %d objects\r\nInformation below\r\n",num+1);
            //uart_sendStr("\robjects at ");
        uart_sendStr(msg);
        if(num!=0)
        {
            //   num--;
        }
        int small=1000;
        while(num>=0)
        {

            sprintf(msg,"Object %d: Starting angle: %d, Width: %d, Ending angle: %d, Distance bot: %d\r\n",  num+1,found[num].start_angle,found[num].width,found[num].end_angle,found[num].distance);
            uart_sendStr(msg);
            if(found[num].end_angle-found[num].start_angle==0)
            {
                // found[num].end_angle+=1;
                // found[num].dist_2=found[num].dist_1;
            }

            float t1=(pow(found[num].dist_1,2)+pow(found[num].dist_2,2));//a^2+b^2
            float t2=(2*(found[num].dist_1)*(found[num].dist_2));//2ab
            float val=(found[num].end_angle-found[num].start_angle)*3.1415/180;
            t2*=cos(val);
            t1-=t2;//-2abcosC
            t1=sqrt(t1);//=c^2
            //t1+=5;
            if(found[num].end_angle-found[num].start_angle==0)
            {

                t1=0;
            }
            if(t1!=0&&t1<small)
            {
                move_servo(found[num].start_angle);
                small=t1;
            }
            sprintf(msg,"True width: %.2f",t1);
            uart_sendStr(msg);
            uart_sendChar('\r');
            uart_sendChar('\n');
            num--;

        }
}



