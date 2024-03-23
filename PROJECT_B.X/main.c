#include "config_bits.h"
#include "sys_config.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>

/* GLOBAL VARIABLES FOR THE INTERRUPTS */
volatile uint16_t val;
volatile uint8_t dutyCycle;
 uint8_t ocChannel=3;

/* INTERRUPT CALLBACK PWM,OUTPUT DESIRED DUTY-CYCLE  */
void __ISR (_TIMER_2_VECTOR, IPL6SRS) T2Interrupt(void)
{
    dutyCycle = (val * 100)/1023;
    ConfigDutyCycle(ocChannel, dutyCycle);
    ClearIntFlagTimer2();
}
/* INTERRUPT CALLBACK TIMER3, READ ADC VALUE */
void __ISR (_TIMER_3_VECTOR, IPL5SRS) T3Interrupt(void)
{
    ADC_start();
    while(ADC_IF() == 0);
    val = ADC_read();
    IFS1bits.AD1IF = 0; // Reset interrupt ADC
    ClearIntFlagTimer3();
}


int main(void){
    uint8_t adcChannel=1;
    /* ------- SETUP INTERRUPTS ------- */
    /* Set Interrupt Controller for multi-vector mode */
    INTCONSET = _INTCON_MVEC_MASK;
    
    /* Enable Interrupt Exceptions */
    // set the CP0 status IE bit high to turn on interrupts globally
    __builtin_enable_interrupts();
    /* -------------------------------- */
    /* ------- SETUP UART ------- */
    if(UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1; // If Led active error initializing UART
        while(1);
    }
    __XC_UART = 1; /* Redirect stdin/stdout/stderr to UART1*/
    /* -------------------------- */
    /* ------- SETUP ADC ------- */
    ADC_init();           
    while(ADC_input(adcChannel) == -1)
    {
    PutChar('T');
    }             
    ADC_enable();
    /* ------------------------- */
    /* ------- SETUP TIMERS & PWM ------- */
    ConfigTimer1(200,1);
    //ConfigTimer2(PWM_FREQ_HZ, 1, 0);    // Output 
    //ConfigTimer3(SAMPL_FREQ_HZ, 1);     // Input     
    StartTimer1();
    //StartTimer2();
    //StartTimer3();             
    //ConfigPWM(ocChannel,2,50);        // OCx, Timer2, 50% duty cycle
    /* ---------------------------------- */
     /* ------- VARIABLES ------- */
    uint8_t choice = 0;     //Used for the Case option
    uint8_t Temp=10;        //Variable to hold the temperature value.
    uint8_t UserInput;          //Variable to hold the input integer.
    uint8_t Value;          //Variable to hold the sequence of numbers.
    uint8_t OptionChoice;   //Variable to identify if its to write a menu choice or value
    uint8_t DesiredLoc;     //Variable to identify which allowed variable the user wants to alter.
    while(1){
        //PutStringn("Problem Timer?");
        if(IFS0bits.T1IF==1){
            Temp=Temp+1;
            printf("\e[1;1H\e[2J");                 //Clear screen
            PutString("Temperature: ");
            PutInt(Temp);
            PutStringn(" ºC");
            /*
            switch(choice){
                case 0:
                    PutStringn("-----MENU----");
                    PutStringn("1-Show PID weights");
                    PutStringn("2-Input Desired Temp");  
                    PutString("Choice: ");
                    break;
                case 2:
                    PutString("Desired Temperature: ");
                
                    break;
                case 3:
                    break;
                default:
                    break;
                };
            //Read Input User Value
            UserInput=GetInteger();
            if(UserInput != 10){
                Value=Value+UserInput*10;
                PutInt(Value);
            }
            else{
                if(OptionChoice==1){
                   choice=Value;
                }
                else{
                    switch(DesiredLoc){         // Case for the desired location of the user value.
                        case 1:
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        default:
                            break;
                   };
                }
                Value=0;            //Reset Input Value for next read.
            }*/
            IFS0bits.T1IF=0;
        }
    }
    return 0;
}