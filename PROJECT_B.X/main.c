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
#include "pi.h"


/* GLOBAL VARIABLES FOR THE INTERRUPTS */
volatile float val;                     /**< Variable to hold the value read by the ADC. */
volatile uint8_t dutyCycle;             /**< Variable to hold the duty cycle of the PWM. */ 
volatile uint8_t choice = 0;            /**< Variable to hold the user's choice. */
volatile uint8_t value = 0;             /**< Variable to hold the sequence of numbers. */
volatile uint8_t minTempRead = 100;     /**< Minimum temperature read by the sensor on a x period of time. */
volatile uint8_t maxTempRead = 0;       /**< Maximum temperature read by the sensor. */
volatile int temp_thermo = 0;           /**< Variable to hold the temperature read by the thermocouple. */
volatile int temp_thermi = 0;           /**< Variable to hold the temperature read by the thermistor. */
volatile int temp = 0;                  /**< Variable to hold the temperature value desired. */
volatile int total_temp = 0;            /**< Variable to hold the total temperature value. */
volatile uint8_t piEn = 0;              /**< Variable to enable the PI controller. */
volatile uint8_t ocChannel = 1;         /**< Variable to hold the output compare channel. */
volatile uint8_t ocChannelControl = 2;  /**< Variable to hold the output compare channel for the control. */
volatile uint32_t time_uc;              /**< Variable to hold the microcontroller's time. */
volatile float dutyCycleControl = 5;   // Variable to hold the duty cycle of the control.
PI pi;                                  /**< PI controller structure. */                      

/* FUNCTION TO READ THE MICROCONTROLLER'S TIME */
/* 
uint32_t ReadCoreTimer(void){
    volatile uint32_t value;
    asm volatile("mfc0 %0,$9" : "=r"(value));
    return value;
} 
*/

/* FUNCTION TO DELAY IN MILLISECONDS */
void delay_ms(int ms){
    for(int i = 0; i < ms; i++){
        for(int j = 0; j < 1000; j++);
    }
}

/* INTERRUPT CALLBACK PWM, OUTPUT DESIRED DUTY-CYCLE  */
void __ISR (_TIMER_2_VECTOR, IPL5SOFT) T2Interrupt(void)
{
    if(piEn == 1){
        // Update the PI controller (PID)
        float y = PI_Update(&pi, temp, total_temp);

        // Normalize the value to a 0-100% range
        float x = (y - 45) / 50;
        dutyCycle = x * 100;
    }
    else{
        dutyCycle = 0;
    }

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
    
    // Rescale the value to the temperature
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
    temp_thermi = val;

    total_temp = temp_thermi + temp_thermo;

    if(total_temp > maxTempRead){
        maxTempRead = total_temp;
    }

    if(total_temp < minTempRead){
        minTempRead = total_temp;
    }

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
    
    ConfigPWM(ocChannel,2,0);              // OCx, Timer2, 50% duty cycle
    ConfigPWM(ocChannelControl,2,5);       // OCx, Timer2, 0% duty cycle
    /* ------------------------------------------------ */

    /* ------------------ SETUP PID ------------------- */
    PI_Init(&pi);

    pi.kp = 8;
    pi.ki = 0.7;
    pi.Ts = 1.0/PWM_FREQ_HZ;

    /* ------------------ VARIABLES ------------------- */            
    
    uint8_t userInput;              // Variable to hold the input integer.                
    int total = -1;
    uint8_t optionChoice = 1;       // Variable to identify if its to write a menu choice or value
    uint8_t desiredLoc;             // Variable to identify which allowed variable the user wants to alter.
    uint8_t k = 0;
    
    /* ------------------------------------------------ */


    PutStringn("Start!");
    delay_ms(100);
    
    // In case of a reset, send the end of frame to the script
    PutStringn("downup");
    delay_ms(100);

    // Send the initial frame via UART, so the script can identify the start of the program
    // to start the data collection (updown)
    PutString("updown");


    while(1){        
        
        ConfigDutyCycle(ocChannelControl, dutyCycleControl);
        

        // Timer 4 & 5 (32bit mode) to print the menu (Polling Method)
        if(GetIntFlagTimer5()){
            DefaultMenu(total_temp, minTempRead, maxTempRead);      // Print the default menu (Temp)
            Menu(choice, value, pi.kp, pi.ki);                // Print the menu with the choice and value 
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
                switch(choice){
                    case 1:
                        
                        if(total == 0){
                            temp = 0;
                            piEn = 0;
                            dutyCycleControl = 0;
                            PutStringn("\n\n\rPID Controller Disabled! - 0ºC Setpoint");
                            delay_ms(2000);
                        }
                        else if(total >= MIN_DESIRED_TEMP && total <= MAX_DESIRED_TEMP){
                            temp = total;
                            piEn = 1;
                            dutyCycleControl = ((temp-45.0)/50.0) * 100.0;
                            
                            PutString("\n\n\rPID Controller Enabled! - ");
                            PutInt(temp);
                            PutString("ºC Setpoint");
                            PutString("\n\rDuty Cycle Control: ");
                            PutIntn(dutyCycleControl);
                            delay_ms(2000);
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
                            PutStringn(" (Disabled PID Controller)");
                            
                            dutyCycleControl = 0;
                            temp = 0;
                            piEn = 0;
                            
                            delay_ms(2000);
                        }
                        break;
                    case 2:
                        break;
                    case 3:
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