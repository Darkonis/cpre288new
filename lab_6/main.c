#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1






   void main() {
    lcd_init();
    uart_init();
    button_init();
   // uint8_t btn=0;
   // int isPressed=0;

 //   uart_sendStr("Sup");
//  uart_sendChar('T');
    while(1)
    {
        //btn = button_getButton();
       /* if(btn!=0&&!isPressed)
        {
            if(btn==6)
            {
                uart_sendStr("Yes\r");
            }
            if(btn==5)
            {
                uart_sendStr("No\r");
            }
            if(btn==4)
            {
                uart_sendStr("Blue, no green, Ahhhhh!!!\r");
            }
            if(btn==3)
            {
                uart_sendStr("What is the speed of an average swallow in flight\r");
            }
            if(btn==2)
            {
                uart_sendStr("Two can be as bad as one it's the loneliest number since the number 1...\r");
            }
            if(btn==1)
            {
                uart_sendStr("One is the loneliest number that there ever was...\r");
            }



            isPressed=1;
        }

        if(!btn)
        {
           isPressed=0;
        }
        */
        int i = get_dist();
        //float t=-28.9*log(i)+235;
        float t=((1.0627*(1.0/(10*10*10*10*10)))*(i*i));
         t+=(-(.0545*i)+79.28);
         if(t>46&&i<680) //we know the error at certain points and this adjusts that according me
         {
             t+=3;
         }
         else if(t>46&&i<750)
                  {
                      t+=1;
                  }
         if(t>21&&t<43.8)
         {
                t-=2;

         }
         else if(t>9&&t<19)
         {
             t+=1;
         }
         //t/=2;
         //t+=6;
        if(i!=0)
        {
            //uart_sendStr();
            lcd_printf("data: %d, %.2f\n",i,t);
            //lcd_printf("val:%f",(float)i/140);
            timer_waitMillis(300);
        }

      //  uart_sendChar('T');
   //uart_receive();
    }
}
   /*
    * val 1 9 cm=2800
    * val 2 20 1700
    * val 3 30 1100
    * val 4 40 850
    * val 5 50 700
    * val 6 15 1920
    * val 7 25 1280
    * val 8 35 920
    * val 9 45 750
    */




