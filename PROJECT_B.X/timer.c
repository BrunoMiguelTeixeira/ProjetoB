/*
 * File: timer.c
 *
 * Sistemas de Instrumentação Electrónica - 2023/24
 *
 * Trabalho B
 * 
 * Autores:
 * - Bruno Teixeira , NMec 98244
 * - Goncalo Rodrigues, NMec 98322
 *
 */


#include "timer.h"
#include "sys_config.h"
#include <xc.h>
#include <stdint.h>

#define PRE_1       1
#define PRE_2       2
#define PRE_4       4
#define PRE_8       8
#define PRE_16      16
#define PRE_32      32
#define PRE_64      64
#define PRE_256     256

struct TimerConfig timer1 = {0,0,0,0};
struct TimerConfig timer2 = {0,0,0,0};
struct TimerConfig timer3 = {0,0,0,0};
struct TimerConfig timer4 = {0,0,0,0};
struct TimerConfig timer5 = {0,0,0,0};

// Timer 1 configuration
void ConfigTimer1(uint32_t targetFreq){
    
    uint16_t prescaler = 0;
    uint32_t fout_presc = 0;

    T1CON = 0;                      // Clear Timer 1 configuration

    // Interrupt configuration
    IFS0bits.T1IF = 0;              // Clear Timer 1 interrupt flag
    IPC1bits.T1IP = 5;              // Set Timer 1 interrupt priority
    IEC0bits.T1IE = 0;              // Enable Timer 1 interrupt

    // ----- Automatic calculation of the values for the Timer 1 configuration -----
    prescaler = PBCLOCK / ((65535+1) * targetFreq); // Calculate the prescaler value

    // Round prescaler to the nearest power of 2 accordig to the timerA prescalers
    if (prescaler <= PRE_1)         {prescaler = PRE_1;   T1CONbits.TCKPS = 0;}
    else if (prescaler <= PRE_8)    {prescaler = PRE_8;   T1CONbits.TCKPS = 1;}
    else if (prescaler <= PRE_64)   {prescaler = PRE_64;  T1CONbits.TCKPS = 2;}
    else                            {prescaler = PRE_256; T1CONbits.TCKPS = 3;}

    fout_presc = PBCLOCK / prescaler;   // Calculate the frequency after prescaler
    // -----------------------------------------------------------------------------

    PR1 = (fout_presc / targetFreq) - 1;  // Calculate the period register value

    timer1.prescaler = prescaler;
    timer1.targetFreq = targetFreq;
    timer1.PR = PR1;
    timer1.timer32bit = 0;
}

void StartTimer1(){
    TMR1 = 0;                   // Clear Timer 1 counter
    T1CONbits.ON = 1;           // Turn on Timer 1
}

void StopTimer1(){
    T1CONbits.ON = 0;           // Turn off Timer 1
}

uint8_t GetIntFlagTimer1(){
    return IFS0bits.T1IF;
}

void ClearIntFlagTimer1(){
    IFS0bits.T1IF = 0;          // Clear Timer 1 interrupt flag
}

struct TimerConfig GetTimer1Config(){
    return timer1;
}


// Timer 2 configuration
void ConfigTimer2(uint32_t targetFreq, uint8_t timer32bit){
    
    uint16_t prescaler = 0;
    uint32_t fout_presc = 0;

    T2CON = 0;                  // Clear Timer 2 configuration
    T3CON = 0;                  // Stop any 16-bit Timer3 operation

    // Interrupt configuration
    IFS0bits.T2IF = 0;          // Clear Timer 2 interrupt flag
    IPC2bits.T2IP = 5;          // Set Timer 2 interrupt priority
    IEC0bits.T2IE = 1;          // Enable Timer 2 interrupt

    if(timer32bit == 1){
        T2CONbits.T32 = 1;          // Enable 32-bit Timer 2 operation
    }

    // ----- Automatic calculation of the values for the Timer 2 configuration -----
    if(timer32bit == 1){
        prescaler = PBCLOCK / ((2^32) * targetFreq); // Calculate the prescaler value
    }
    else{
        prescaler = PBCLOCK / ((2^16) * targetFreq); // Calculate the prescaler value
    }

    // Round prescaler to the nearest power of 2 accordig to the timerB prescalers
    if (prescaler <= PRE_1)         {prescaler = PRE_1;   T2CONbits.TCKPS = 0;}
    else if (prescaler <= PRE_2)    {prescaler = PRE_2;   T2CONbits.TCKPS = 1;}
    else if (prescaler <= PRE_4)    {prescaler = PRE_4;   T2CONbits.TCKPS = 2;}
    else if (prescaler <= PRE_8)    {prescaler = PRE_8;   T2CONbits.TCKPS = 3;}
    else if (prescaler <= PRE_16)   {prescaler = PRE_16;  T2CONbits.TCKPS = 4;}
    else if (prescaler <= PRE_32)   {prescaler = PRE_32;  T2CONbits.TCKPS = 5;}
    else if (prescaler <= PRE_64)   {prescaler = PRE_64;  T2CONbits.TCKPS = 6;}
    else                            {prescaler = PRE_256; T2CONbits.TCKPS = 7;}

    fout_presc = PBCLOCK / prescaler;   // Calculate the frequency after prescaler
    // -----------------------------------------------------------------------------

    PR2 = (fout_presc / targetFreq) - 1;  // Calculate the period register value

    timer2.prescaler = prescaler;
    timer2.targetFreq = targetFreq;
    timer2.PR = PR2;
    timer2.timer32bit = timer32bit;
}

void StartTimer2(){
    TMR2 = 0;                   // Clear Timer 2 counter
    T2CONbits.ON = 1;           // Turn on Timer 2
}

void StopTimer2(){
    T2CONbits.ON = 0;           // Turn off Timer 2
}

uint8_t GetIntFlagTimer2(){
    return IFS0bits.T2IF;
}

void ClearIntFlagTimer2(){
    IFS0bits.T2IF = 0;          // Clear Timer 2 interrupt flag
}

struct TimerConfig GetTimer2Config(){
    return timer2;
}


// Timer 3 configuration
void ConfigTimer3(uint32_t targetFreq){
        
    uint16_t prescaler = 0;
    uint32_t fout_presc = 0;

    T3CON = 0;                  // Clear Timer 3 configuration~

    // Interrupt configuration
    IFS0bits.T3IF = 0;          // Clear Timer 3 interrupt flag
    IPC3bits.T3IP = 5;          // Set Timer 3 interrupt priority
    IEC0bits.T3IE = 1;          // Enable Timer 3 interrupt

    // ----- Automatic calculation of the values for the Timer 3 configuration -----
    prescaler = PBCLOCK / ((65535+1) * targetFreq); // Calculate the prescaler value

    // Round prescaler to the nearest power of 2 accordig to the timerB prescalers
    if (prescaler <= PRE_1)         {prescaler = PRE_1;   T3CONbits.TCKPS = 0;}
    else if (prescaler <= PRE_2)    {prescaler = PRE_2;   T3CONbits.TCKPS = 1;}
    else if (prescaler <= PRE_4)    {prescaler = PRE_4;   T3CONbits.TCKPS = 2;}
    else if (prescaler <= PRE_8)    {prescaler = PRE_8;   T3CONbits.TCKPS = 3;}
    else if (prescaler <= PRE_16)   {prescaler = PRE_16;  T3CONbits.TCKPS = 4;}
    else if (prescaler <= PRE_32)   {prescaler = PRE_32;  T3CONbits.TCKPS = 5;}
    else if (prescaler <= PRE_64)   {prescaler = PRE_64;  T3CONbits.TCKPS = 6;}
    else                            {prescaler = PRE_256; T3CONbits.TCKPS = 7;}

    fout_presc = PBCLOCK / prescaler;   // Calculate the frequency after prescaler
    // -----------------------------------------------------------------------------

    PR3 = (fout_presc / targetFreq) - 1;  // Calculate the period register value

    timer3.prescaler = prescaler;
    timer3.targetFreq = targetFreq;
    timer3.PR = PR3;
    timer3.timer32bit = 0;
}

void StartTimer3(){
    TMR3 = 0;                   // Clear Timer 3 counter
    T3CONbits.ON = 1;           // Turn on Timer 3
}

void StopTimer3(){
    T3CONbits.ON = 0;           // Turn off Timer 3
}

uint8_t GetIntFlagTimer3(){
    return IFS0bits.T3IF;
}

void ClearIntFlagTimer3(){
    IFS0bits.T3IF = 0;          // Clear Timer 3 interrupt flag
}

struct TimerConfig GetTimer3Config(){
    return timer3;
}

// Timer 4 configuration
void ConfigTimer4(uint32_t targetFreq, uint8_t timer32bit){
    
    uint16_t prescaler = 0;
    uint32_t fout_presc = 0;

    T4CON = 0;                  // Clear Timer 4 configuration
    T5CON = 0;                  // Stop any 16-bit Timer5 operation

    // Interrupt configuration
    IFS0bits.T4IF = 0;          // Clear Timer 4 interrupt flag
    IPC4bits.T4IP = 5;          // Set Timer 4 interrupt priority
    IEC0bits.T4IE = 0;          // Enable Timer 4 interrupt
    
    if(timer32bit == 1){
        T4CONbits.T32 = 1;          // Enable 32-bit Timer 4 operation
    }

    // ----- Automatic calculation of the values for the Timer 4 configuration -----
    if(timer32bit == 1){
        prescaler = PBCLOCK / ((2^32) * targetFreq); // Calculate the prescaler value
    }
    else{
        prescaler = PBCLOCK / ((2^16) * targetFreq); // Calculate the prescaler value
    }

    // Round prescaler to the nearest power of 2 accordig to the timerB prescalers
    if (prescaler <= PRE_1)         {prescaler = PRE_1;   T4CONbits.TCKPS = 0;}
    else if (prescaler <= PRE_2)    {prescaler = PRE_2;   T4CONbits.TCKPS = 1;}
    else if (prescaler <= PRE_4)    {prescaler = PRE_4;   T4CONbits.TCKPS = 2;}
    else if (prescaler <= PRE_8)    {prescaler = PRE_8;   T4CONbits.TCKPS = 3;}
    else if (prescaler <= PRE_16)   {prescaler = PRE_16;  T4CONbits.TCKPS = 4;}
    else if (prescaler <= PRE_32)   {prescaler = PRE_32;  T4CONbits.TCKPS = 5;}
    else if (prescaler <= PRE_64)   {prescaler = PRE_64;  T4CONbits.TCKPS = 6;}
    else                            {prescaler = PRE_256; T4CONbits.TCKPS = 7;}

    fout_presc = PBCLOCK / prescaler;   // Calculate the frequency after prescaler
    // -----------------------------------------------------------------------------

    PR4 = (fout_presc / targetFreq) - 1;  // Calculate the period register value
    
    timer4.prescaler = prescaler;
    timer4.targetFreq = targetFreq;
    timer4.PR = PR4;
    timer4.timer32bit = timer32bit;
}

void StartTimer4(){
    TMR4 = 0;                   // Clear Timer 4 counter
    T4CONbits.ON = 1;           // Turn on Timer 4
}

void StopTimer4(){
    T4CONbits.ON = 0;           // Turn off Timer 4
}

uint8_t GetIntFlagTimer4(){
    return IFS0bits.T4IF;
}

void ClearIntFlagTimer4(){
    IFS0bits.T4IF = 0;          // Clear Timer 4 interrupt flag
}


// Timer 5 configuration
void ConfigTimer5(uint32_t targetFreq){
    
    uint16_t prescaler = 0;
    uint32_t fout_presc = 0;

    T5CON = 0;                  // Clear Timer 5 configuration

    // Interrupt configuration
    IFS0bits.T5IF = 0;          // Clear Timer 5 interrupt flag
    IPC5bits.T5IP = 5;          // Set Timer 5 interrupt priority
    IEC0bits.T5IE = 0;          // Enable Timer 5 interrupt

    // ----- Automatic calculation of the values for the Timer 5 configuration -----
    prescaler = PBCLOCK / ((65535+1) * targetFreq); // Calculate the prescaler value

    // Round prescaler to the nearest power of 2 accordig to the timerB prescalers
    if (prescaler <= PRE_1)         {prescaler = PRE_1;   T5CONbits.TCKPS = 0;}
    else if (prescaler <= PRE_2)    {prescaler = PRE_2;   T5CONbits.TCKPS = 1;}
    else if (prescaler <= PRE_4)    {prescaler = PRE_4;   T5CONbits.TCKPS = 2;}
    else if (prescaler <= PRE_8)    {prescaler = PRE_8;   T5CONbits.TCKPS = 3;}
    else if (prescaler <= PRE_16)   {prescaler = PRE_16;  T5CONbits.TCKPS = 4;}
    else if (prescaler <= PRE_32)   {prescaler = PRE_32;  T5CONbits.TCKPS = 5;}
    else if (prescaler <= PRE_64)   {prescaler = PRE_64;  T5CONbits.TCKPS = 6;}
    else                            {prescaler = PRE_256; T5CONbits.TCKPS = 7;}

    fout_presc = PBCLOCK / prescaler;   // Calculate the frequency after prescaler
    // -----------------------------------------------------------------------------

    PR5 = (fout_presc / targetFreq  ) - 1;  // Calculate the period register value
    timer5.prescaler = prescaler;
    timer5.targetFreq = targetFreq;
    timer5.PR = PR5;
    timer5.timer32bit = 0;
}

void StartTimer5(){
    TMR5 = 0;                   // Clear Timer 5 counter
    T5CONbits.ON = 1;           // Turn on Timer 5
}

void StopTimer5(){
    T5CONbits.ON = 0;           // Turn off Timer 5
}

uint8_t GetIntFlagTimer5(){
    return IFS0bits.T5IF;
}

void ClearIntFlagTimer5(){
    IFS0bits.T5IF = 0;          // Clear Timer 5 interrupt flag
}

struct TimerConfig GetTimer5Config(){
    return timer5;
}
