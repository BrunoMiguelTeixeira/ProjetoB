#ifndef __ADC_H__
#define __ADC_H__


typedef enum{AN0=0,AN1,AN2,AN3,AN4,AN5,AN6,AN7,AN8,AN9,AN10,AN11,AN12,AN13,AN14,AN15
}ADCchannels;


/**
 * Function: ADC_init()\n
 * Precondition:\n
 * Input: None\n
 * Output: None\n
 * Side Effects: None\n
 * Overview: Starts ADC with predetermined options and settings\n
 * Note: None\n
*/
void ADC_init(void);

/**
 * Function: ADC_input() \n
 * Precondition: \n
 * Input: Unsigned integer\n
 * Output: Integer\n
 * Side Effects: None\n
 * Overview: Function to choose ADC output pin\n
 * Note: None\n
*/
int ADC_input(uint16_t a);


/**
 * Function: ADC_enable() \n
 * Precondition:\n
 * Input: None \n
 * Output: None \n
 * Side Effects: None \n
 * Overview: Function enables ADC\n
 * Note: None\n
*/
void ADC_enable(void);

/**
 * Function: ADC_read()\n
 * Precondition: \n
 * Input: None\n
 * Output: Float\n
 * Side Effects: None\n
 * Overview: Function reads ADC BUF0 and outputs has float\n
 * Note: None\n
 */
uint16_t ADC_read(void);

/**
 * Function: ADC_IF()\n
 * Precondition:\n
 * Input: None\n
 * Output: Unsigned Integer\n
 * Side Effects: None\n
 * Overview: Resets ADC Interruption Flag\n
 * Note: None\n
 */
uint8_t ADC_IF(void);

/**
 * Function: ADC_start()\n
 * Precondition:\n
 * Input: None\n
 * Output: None\n
 * Side Effects: None\n
 * Overview: ADC starts conversion\n
 * Note: None\n
*/
void ADC_start(void);
#endif