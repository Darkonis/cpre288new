/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable
void uart_init(void){

      //while((SYSCTL_PRGPIO_R&0x03) == 0);
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
          //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
          SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
          //enable alternate functions on port b pins 0 and 1
          GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);
          //enable Rx and Tx on port C on pins 4 and 5
          GPIO_PORTB_PCTL_R |= 0x00000011;
          //set pin 4 and 5 to digital
          GPIO_PORTB_DEN_R |= (BIT1 | BIT0);
          //set pin 0 to Rx or input
          GPIO_PORTB_DIR_R &= ~BIT0;
          //set pin 1 to Tx or output
          GPIO_PORTB_DIR_R |= BIT1;
        //  uint16_t iBRD = 104;//use equations
         // uint16_t fBRD = 11;//use equations
         uint16_t iBRD = 8;//use equations 16000000/(115200*16) =8.68055555 wifi
                 uint16_t fBRD = 44;//use equations 0.68055555555*64+.5 = 44.0555
           //turn off uart1 while we set it up
           UART1_CTL_R &= ~(UART_CTL_UARTEN);
           //set baud rate
           UART1_IBRD_R = iBRD;
           UART1_FBRD_R = fBRD;
           //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
           UART1_LCRH_R = UART_LCRH_WLEN_8 ;
           //use system clock as source
           UART1_CC_R = UART_CC_CS_SYSCLK;
           //re-enable enable RX, TX, and uart1
           UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
         //Below is interupt setup

         UART1_CTL_R &= ~(UART_CTL_UARTEN);
          //clear interrupt flags
          UART1_ICR_R = (UART_ICR_TXIC | UART_ICR_RXIC);
          //enable send and receive raw interrupts
          UART1_IM_R |= UART_IM_TXIM | UART_IM_RXIM;
          //set priority of usart1 interrupt to 1. group 1 bits 21-23
          NVIC_PRI4_R |= 0x00200000;
          //enable interrupt for IRQ 6 set bit 6
          NVIC_EN0_R |= 0x00000040;
          //tell cpu to use ISR handler for uart1
          IntRegister(INT_UART1, UART1_Handler);
          //enable global interrupts


          //re-enable enable RX, TX, and uart1
          UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
       //enable ai10
       //GPIO_PORTB_DIR_R |= BIT4;
       //Enable ADC0 using SS0:
       //enable ADC 0 module on port D

       SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
       //enable clock for ADC
       SYSCTL_RCGCADC_R |= 0x1;
       //enable port D pin 0 to work as alternate functions
       GPIO_PORTD_AFSEL_R |= 0x01;
       //set pin to input - 0
       GPIO_PORTD_DEN_R &= 0b11111110;
       //disable analog isolation for the pin
       GPIO_PORTD_AMSEL_R |= 0x01;
       //initialize the port trigger source as processor (default)
       GPIO_PORTD_ADCCTL_R = 0x00;

      //Below is interupt setup


        //disable SS0 sample sequencer to configure it
        ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
        //initialize the ADC trigger source as processor (default)
        ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
        //set 1st sample to use the AIN10 ADC pin
        ADC0_SSMUX0_R |= 0x000A;
        //enable raw interrupt status
        ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
        //enable oversampling to average
        ADC0_SAC_R |= ADC_SAC_AVG_64X;
        //re-enable ADC0 SS0
        ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

        ADC0_ISC_R |= ADC_ISC_IN0;
        //enable ADC0SS0 interrupt
        ADC0_IM_R |= ADC_IM_MASK0;
        //enable interrupt for IRQ 14 set bit 14
        NVIC_EN0_R |= 0x00004000;
        //tell cpu to use ISR handler for ADC0SS0
        IntRegister(INT_ADC0SS0, ADC0SS0_Handler);

        IntMasterEnable();
}
char in[20];
int index=0;
void uart_sendChar(char data){
    while(UART1_FR_R& 0x20)
    {

    }
    UART1_DR_R=data;
    //char tmp=(char)index+49;

  //  lcd_printf("%d %c",index,data);
    if(data=='\r')
    {
        UART1_DR_R='\n';
       // lcd_printf(in);
        while (index!=0)
        {

            in[index]='\0';
            index--;
        }
        in[index]='\0';
    }
    else if(index==20)
    {
        UART1_DR_R ='\n';
        UART1_DR_R ='\r';
        //lcd_printf(in);
        while (index!=0)
        {

            in[index]='\0';
            index--;
        }
        in[index]='\0';
    }
    else
    {
        in[index]=data;
        index++;
    }

}

char uart_receive(void){
char data =0;
while (UART1_FR_R&UART_FR_RXFE )
    {
    //return;
    }
data  = (char)(UART1_DR_R & 0xFF);
uart_sendChar(data);
//uart_sendChar('\r');
//uart_sendChar('\n');
return  data;
}

void uart_sendStr(const char *data){
    int i=0;
    while(data[i]!='\0')
    {
        uart_sendChar(data[i]);
       // uart_receive();
        i++;
    }
}
volatile int data=0;
//volatile int index=0;
int get_dist()
{
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
    //initiate SS0 conversion
    ADC0_PSSI_R=ADC_PSSI_SS0;
    //wait for ADC conversion to be complete
    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
    //wait
    }
    //clear interrupt
    ADC0_ISC_R=ADC_ISC_IN0;

            //float t=-28.9*log(i)+235;
            float t=((1.0627*(1.0/(10*10*10*10*10)))*(data*data));
             t+=-(.0545*data)+79.28;
             if(t>46&&data<680) //we know the error at certain points and this adjusts that according me
             {
                 t+=3;
             }
             else if(t>46&&data<750)
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
    return t;

}

void  ADC0SS0_Handler()
{
        ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
        //initiate SS0 conversion
        ADC0_PSSI_R=ADC_PSSI_SS0;
        //wait for ADC conversion to be complete
        while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
        //wait
        }
        //clear interrupt
        ADC0_ISC_R=ADC_ISC_IN0;
        data = ADC0_SSFIFO0_R;


}
void UART1_Handler(void){
//received a byte
if(UART1_MIS_R & UART_MIS_RXMIS){

   // uart_receive();
    //uart_sendChar('\r');


UART1_ICR_R |= UART_ICR_RXIC; //clear interrupt
}
//sent a byte
else if(UART1_MIS_R & UART_MIS_TXMIS){

UART1_ICR_R |= UART_ICR_TXIC; //clear interrupt
}
}
