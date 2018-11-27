#include "movement.h"
void manuel_control()
{
    int byte =uart_receive();
                         uart_sendChar(byte);//send the byte
                         uart_sendChar('\r');//send the byte
                         if((char)byte=='w')//based on input change what its doing
                         {
                             oi_setWheels(500, 500); // forward
                         }
                         if((char)byte=='s')
                                    {
                                        oi_setWheels(-500, -500); //left
                                    }
                         if((char)byte=='a')
                                    {
                                        oi_setWheels(500, -500); //right
                                    }
                         if((char)byte=='d')
                         {
                             oi_setWheels(-500, 500);//backward
                         }
                         if((char)byte=='x')
                                     {
                                         oi_setWheels(0,0);//stop
                                     }

                         lcd_printf("%c\n",(char)byte);

                         // uint8_t button = button_getButton();
                        //  lcd_printf("Button: %d", button);
                         /* if(button!=0)
                          {

                            //  cyBot_sendByte((char)button+48);//send the byte

                              //cyBot_UART1_Handler();
                              //cyBot_uart_flush();
                          }
                         */
}

void turn_clockwise(int power)
{
    oi_setWheels(power, -power); //right
}
void turn_counterclockwise(int power)
{
    oi_setWheels(-power, power); //right
}
void turn(power)
{
    if(power<0)
    {
        turn_counterclockwise(power);
    }
    else
    {
        turn_clockwise(power);
    }
}
void turn_angle(int angle,int power, oi_t *sensor_data )
{
    int tolerance=20;
    int power2 =power;
    int sum=0;
    int tmp2=0;
    oi_update(sensor_data);
    if(angle<0)
            {
             power=-power;
            }
    while(abs(tmp2-abs(angle))>tolerance)
    {

        tmp2 += (sensor_data->angle);
        int tmp =abs(tmp2-angle);

         sum=abs(tmp2-abs(angle));
         char msg[100];
         sprintf(msg, "angle %d\n", sum);
         lcd_printf(msg);
         sprintf(msg, "angle %d\n\r", sum);
         uart_sendStr(msg);

         turn(power);
       //  timer_waitMillis(500);
       //  oi_setWheels(0,0);//stop
        oi_update(sensor_data);
    }

}
