#include <xc.h>

void ADC_init(void){
    AD1CON1bits.SSRC = 7;   //Auto convert option
    AD1CON1bits.CLRASAM = 1;//Auto-Sample Start bit
    AD1CON1bits.FORM = 0;   //choose the output format -> Integer 16 bit
    AD1CON2bits.VCFG = 0;   //Voltage Reference Configuration bits VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = 0;   // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI} before interruption
    AD1CON3bits.ADRC = 1;   //ADC Clock Source Select bit, internal RC clock
    AD1CON3bits.SAMC = 16;  //set the duration of the auto-sample time,16 clock cycles, each cycle Sample time is 16TAD ( TAD = 100ns)
}

int ADC_input(uint16_t a){
    if(a>15 || a<0){ 
        return -1;
    }
     AD1CHSbits.CH0SA = a; // Select AN a as input for A/D converter
     TRISB=TRISB | (0x80000>>(15-a));//Input-> 1, shifts first bit 1 to required position, "or" to force the 1.
     AD1PCFGbits.PCFG0 = a;
     return 0;
}

void ADC_enable(void){
    AD1CON1bits.ON = 1; // Enable A/D module
}
uint16_t ADC_read(void){
    return (uint16_t)ADC1BUF0;
}

uint8_t ADC_IF(void){
    return IFS1bits.AD1IF;
}

void ADC_start(void){
    IFS1bits.AD1IF = 0; // Reset interrupt flag
    AD1CON1bits.ASAM = 1; // Start conversion
}