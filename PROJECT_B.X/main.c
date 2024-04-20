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
#include "thermo_k.h"


/* GLOBAL VARIABLES FOR THE INTERRUPTS */
volatile float val;                 /**< Variable to hold the value read by the ADC. */
volatile uint8_t dutyCycle;         /**< Variable to hold the duty cycle of the PWM. */ 
volatile uint8_t choice = 0;        /**< Variable to hold the user's choice. */
volatile uint8_t value = 0;         /**< Variable to hold the sequence of numbers. */
volatile uint8_t minTempRead = 0;   /**< Minimum temperature read by the sensor on a x period of time. */
volatile uint8_t maxTempRead = 0;   /**< Maximum temperature read by the sensor. */
volatile int temp_thermo = 0;       /**< Variable to hold the temperature read by the thermocouple. */
volatile int temp = 10;             /**< Variable to hold the temperature value of the thermistor. */
volatile int total_temp = 0;        /**< Variable to hold the total temperature value. */

uint8_t ocChannel = 3;

void delay_ms(int ms){
    for(int i = 0; i < ms; i++){
        for(int j = 0; j < 1000; j++);
    }
}

/* INTERRUPT CALLBACK PWM, OUTPUT DESIRED DUTY-CYCLE  */
void __ISR (_TIMER_2_VECTOR, IPL6SRS) T2Interrupt(void)
{
    dutyCycle = (val * 100) / 1023;
    ConfigDutyCycle(ocChannel, dutyCycle);
    ClearIntFlagTimer2();
}

/* INTERRUPT CALLBACK TIMER3, READ ADC value */
void __ISR (_TIMER_3_VECTOR, IPL5SOFT) T3Interrupt(void)
{

    // Read the value from the ADC (Thermocouple)
    val = 0;
    ADC_input(ADC_CHAN_THERMO);
    ADC_start();
    while(ADC_IF() == 0);
    val = ADC_read();
    
    // Conversion to Millivolts
    val = (val*3300) / 1023;

    // Find the closest value in the array
    int min = 1000;
    int min_index = 0;
    for(int i = 0; i < 151; i++){
        if(abs(val - therm_amp[i]) < min){
            min = abs(val - therm_amp[i]);
            min_index = i;
        }
    }
    
    temp_thermo = min_index - 10;

    if(temp_thermo < 0){
        temp_thermo = 0;
    }

    // Read the value from the ADC (Thermistor)
    val = 0;
    ADC_enable();
    ADC_input(ADC_CHAN_THERMIC);
    ADC_start();
    while(ADC_IF() == 0);
    val = ADC_read();

    // Conversion to Millivolts
    val = (val*3300) / 1023;

    // Conversion to Celsius (Using the linear approximation of the thermistor)
    val = (val/1000) / 0.060445;
    temp = val;

    total_temp = temp + temp_thermo;
    
    IFS1bits.AD1IF = 0; // Reset interrupt ADC
    ClearIntFlagTimer3();
}

int main(void){
    
    // Config LD5 (RC1) as output
    TRISCbits.TRISC1 = 0;       // Set LED5 as output
    LATCbits.LATC1 = 0;         // Turn off LED5

    /* --------------- SETUP INTERRUPTS --------------- */
    /* Set Interrupt Controller for multi-vector mode */
    INTCONSET = _INTCON_MVEC_MASK;
    
    /* Enable Interrupt Exceptions */
    // set the CP0 status IE bit high to turn on interrupts globally
    __builtin_enable_interrupts();
    /* ------------------------------------------------ */

    /* ------------------ SETUP UART ------------------ */
    if(UartInit(PBCLOCK, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1; // If the LED is active, there was an error
        while(1);
    }
    __XC_UART = 1;  /* Redirect stdin/stdout/stderr to UART1*/
    /* ------------------------------------------------ */

    /* ------------------ SETUP ADC ------------------- */
    ADC_init();           
    ADC_input(ADC_CHAN_THERMO); // Set the input channel for the ADC       
    ADC_enable();
    /* ------------------------------------------------ */

    /* -------------- SETUP TIMERS & PWM -------------- */
    ConfigTimer2(PWM_FREQ_HZ, 0);   // Output 
    ConfigTimer3(SAMPL_FREQ_HZ);    // Input 
    ConfigTimer4(3, 1);             // Timer 4, 32 bit mode (Due to 1Hz frequency)

    StartTimer2();
    StartTimer3(); 
    StartTimer4();    
    
    ConfigPWM(ocChannel,2,50);      // OCx, Timer2, 50% duty cycle
    /* ------------------------------------------------ */

    /* ------------------ VARIABLES ------------------- */            
    
    uint8_t userInput;              // Variable to hold the input integer.                
    int total = -1;
    uint8_t optionChoice = 1;       // Variable to identify if its to write a menu choice or value
    uint8_t desiredLoc;             // Variable to identify which allowed variable the user wants to alter.

    PutStringn("Start!");
    
    while(1){        
        
        /*
        if(GetIntFlagTimer3()){
            // Read the value from the ADC
            val = 0;
            ADC_start();
            while(ADC_IF() == 0);
            val = ADC_read();

            // Conversion to Millivolts
            val = (val*3300) / 1023;

            // Find the closest value in the array
            int min = 1000;
            int min_index = 0;
            for(int i = 0; i < 151; i++){
                if(abs(val - therm_amp[i]) < min){
                    min = abs(val - therm_amp[i]);
                    min_index = i;
                }
            }

            temp_therm = min_index;

            IFS1bits.AD1IF = 0; // Reset interrupt ADC
            ClearIntFlagTimer3();
        }
        */
        
        if(GetIntFlagTimer5()){
            DefaultMenu(total_temp);      // Print the default menu (Temp)
            Menu(choice, value);                // Print the menu with the choice and value 
            PORTCbits.RC1 = !PORTCbits.RC1;     // Toggle LED to see the timer's timing
            ClearIntFlagTimer5();
        }
        
        
        
        // Read Input User value
        userInput = GetInteger();
        switch(userInput){
            // 1-9 numbers for the user's choice
            case 0 ... 9:
                value = value*10 + userInput;
                break;

            // Enter key to confirm the choice
            case 10:
                total = value;
                break;
        };

        if(total != -1){
            //The user has entered a value; Check the menu to print
            if(optionChoice == 1){
                choice = total;
                total = -1;
                value = 0;
                optionChoice = 0;
            }else{
                // The user has entered a value; Check the value to print
                switch(choice){
                    case 1:
                        if(total >= MIN_DESIRED_TEMP && total <= MAX_DESIRED_TEMP){
                            temp = total;
                        }
                        else{
                            PutString("\e[0m");     // Reset to default color (white)
                            PutString("\n\n\rInvalid Temperature Value! Please choose a value between (");
                            
                            PutString("\e[31m");    // Print in red
                            PutInt(MIN_DESIRED_TEMP);
                            
                            PutString("\e[0m");     // Reset to default color (white)
                            PutString(" and ");

                            PutString("\e[31m");    // Print in red
                            PutInt(MAX_DESIRED_TEMP);

                            PutString("\e[0m");     // Reset to default color (white)
                            PutStringn(") ºC");

                            delay_ms(2000);
                        }
                        break;
                    case 2:

                        break;
                    case 3:
                        PutStringn("Resetting values...");
                        minTempRead = 0;
                        maxTempRead = 0;
                        break;
                    default:
                        break;
                };
                total = -1;
                value = 0;
                choice = 0;
                optionChoice = 1;
            }
        }

        
        
    } 
    return 0;
}