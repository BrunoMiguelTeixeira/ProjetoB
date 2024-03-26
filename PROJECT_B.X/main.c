#include "config_bits.h"
#include "sys_config.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "menu.h"
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
    ConfigTimer1(1);
    ConfigTimer2(PWM_FREQ_HZ, 0);    // Output 
    ConfigTimer3(SAMPL_FREQ_HZ);     // Input     
    StartTimer1();
    StartTimer2();
    StartTimer3();             
    ConfigPWM(ocChannel,2,50);        // OCx, Timer2, 50% duty cycle
    /* ---------------------------------- */
     /* ------- VARIABLES ------- */
    uint8_t choice = 0;     //Used for the Case option
    uint8_t Temp=10;        //Variable to hold the temperature value.
    uint8_t UserInput;          //Variable to hold the input integer.
    int Value=0;          //Variable to hold the sequence of numbers.
    int Total=-1;
    uint8_t OptionChoice=1;   //Variable to identify if its to write a menu choice or value
    uint8_t DesiredLoc;     //Variable to identify which allowed variable the user wants to alter.
    PutStringn("Start!");
    while(1){
        if(GetIntFlagTimer1()==1){
            DefaultMenu(Temp);
            Menu(choice,Value);
            PORTAbits.RA3 != PORTAbits.RA3;
            ClearIntFlagTimer1();
        }
        //Read Input User Value
        UserInput=GetInteger();
        switch(UserInput){
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                Value=Value*10+UserInput;
                break;
            case 10:
                Total=Value;
        };  

        if(Total>0){
            if(OptionChoice==1){
                choice=Total;
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
            Total=-1;
        }
        }
    return 0;
}