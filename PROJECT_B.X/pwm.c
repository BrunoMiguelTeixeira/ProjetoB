#include "pwm.h"
#include "timer.h"
#include "sys_config.h"
#include <xc.h>
#include <stdint.h>

/*
 * File: uart.h
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


extern struct TimerConfig timer1;
extern struct TimerConfig timer2;
extern struct TimerConfig timer3;
extern struct TimerConfig timer4;
extern struct TimerConfig timer5;

void ConfigDutyCycle(uint8_t ocSelect, uint8_t dutyCycle)
{
    if(ocSelect == 1) { OC1RS = ((timer2.PR + 1) * dutyCycle) / 100; }
    if(ocSelect == 2) { OC2RS = ((timer2.PR + 1) * dutyCycle) / 100; }
    if(ocSelect == 3) { OC3RS = ((timer2.PR + 1) * dutyCycle) / 100; }
    if(ocSelect == 4) { OC4RS = ((timer2.PR + 1) * dutyCycle) / 100; }
    if(ocSelect == 5) { OC5RS = ((timer2.PR + 1) * dutyCycle) / 100; }
}

uint8_t ConfigPWM(uint8_t ocSelect, uint8_t timerSelect, uint8_t dutyCycle)
{
    struct TimerConfig timerSelected;

    if (ocSelect > 5){
        return 0x01;    // OC not available, Exception 1
    }
    if (dutyCycle > 100){
        return 0x02;    // Duty Cycle out of range, Exception 2
    }
    if (timerSelect > 3 && timerSelect < 2){
        return 0x03;    // Timer not available, Exception 3
    }

    switch(ocSelect){
        case 1:
            OC1CON = 0;
            OC1R = 0;

            /* if (timerSelected.timer32bit == 1){
                OC[ocSelect - 1].OCxCONbits.OC32 = 1;                              // 32-bit Timer source
            } */    

            /*  
                ON       = 1: Enable Output Compare
                FRZ      = 0: PWM continues in CPU Freeze mode
                SIDL     = 0
                [12:6]   = 000000
                OC32     = 0: 16-bit Timer source
                [3]      = 0
                OCTSEL   = 0: Timer 2 is the clock source for this OC
                OCM[2:0] = 110: PWM mode on OCx; Fault pin disabled
            */

            OC1RS = ((timer2.PR + 1) * dutyCycle) / 100;
            if (timerSelect == 2) { timerSelected = timer2; OC1CON = 0b1000000000000110;}
            if (timerSelect == 3) { timerSelected = timer3; OC1CON = 0b1000000000001110;}
            break;
        case 2:
            OC2CON = 0;
            OC2R = 0;
            OC2RS = ((timer2.PR + 1) * dutyCycle) / 100;
            if (timerSelect == 2) { timerSelected = timer2; OC2CON = 0b1000000000000110;}
            if (timerSelect == 3) { timerSelected = timer3; OC2CON = 0b1000000000001110;}
            break;
        case 3:
            OC3CON = 0;
            OC3R = 0;
            OC3RS = ((timer2.PR + 1) * dutyCycle) / 100;
            if (timerSelect == 2) { timerSelected = timer2; OC3CON = 0b1000000000000110;}
            if (timerSelect == 3) { timerSelected = timer3; OC3CON = 0b1000000000001110;}
            break;
        case 4:
            OC4CON = 0;
            OC4R = 0;
            OC4RS = ((timer2.PR + 1) * dutyCycle) / 100;
            if (timerSelect == 2) { timerSelected = timer2; OC4CON = 0b1000000000000110;}
            if (timerSelect == 3) { timerSelected = timer3; OC4CON = 0b1000000000001110;}
            break;
        case 5:
            OC5CON = 0;
            OC5R = 0;
            OC5RS = ((timer2.PR + 1) * dutyCycle) / 100;
            if (timerSelect == 2) { timerSelected = timer2; OC5CON = 0b1000000000000110;}
            if (timerSelect == 3) { timerSelected = timer3; OC5CON = 0b1000000000001110;}
            break;
        default:
            return 0x01;    // OC not available, Exception 1
    }
    
    return 0x00;
};