#include <xc.h>
#include <stdint.h>

#ifndef PWM_H
#define PWM_H

/**
 * PWM Configuration for OC1
 * 
 * Configures the PWM for the OC1 pin, by selecting the timer to be used and the duty cycle.
 *
 * @param ocSelect The OC module to be used
 * @param timerSelect The timer to be used for the OC module
 * @param dutyCycle The duty cycle to be used
*/
uint8_t ConfigPWM(uint8_t ocSelect, uint8_t timerSelect, uint8_t dutyCycle);

/**
 * Configures the duty cycle for the OC module
 * 
 * It configures the duty cycle for the OC module, by setting the OCxRS register.
 * This way its faster to change the duty cycle, without having to reconfigure the OC module.
 *
 * @param ocSelect The OC module to be used
 * @param dutyCycle The duty cycle to be used
*/
void ConfigDutyCycle(uint8_t ocSelect, uint8_t dutyCycle);

#endif	/* PWM_H */