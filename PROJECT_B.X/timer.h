#include <xc.h>
#include <stdint.h>

#ifndef TIMER_H
#define TIMER_H


/**
 * TimerConfig struct
 *
 * In order to access easily each Timer's config, this structure was created.
 * It contains the prescaler used, the target frequency, the PR value automatically calculated
 * , the interrupt flag and the timer32bit flag.
 */
struct TimerConfig{
    uint16_t prescaler;     /**< Prescaler used for this specific timer. */
    uint32_t targetFreq;    /**< Target frequency given by the user. */
    uint32_t PR;            /**< PR value automatically calculated by the function. */
    uint8_t timer32bit;     /**< Timer32bit flag: 1 -> 32 bit Timer; 0 -> 16 bit Timer. */
}; 

extern struct TimerConfig timer1;   /**< Timer 1 Config struct. */
extern struct TimerConfig timer2;   /**< Timer 2 Config struct. */
extern struct TimerConfig timer3;   /**< Timer 3 Config struct. */
extern struct TimerConfig timer4;   /**< Timer 4 Config struct. */
extern struct TimerConfig timer5;   /**< Timer 5 Config struct. */

/**
 * Timer 1 Config function
 *
 * This function configures the Timer 1 with the given target frequency and interrupt flag.
 * Taking into account the given frequency, the function calculates the PR value and the prescaler,
 * storing these values inside the TimerConfig struct.
 *
 * @param targetFreq: Target frequency given by the user.
 */
void ConfigTimer1(uint32_t targetFreq);

/**
 * Timer 2 Config function
 *
 * This function configures the Timer 2 with the given target frequency, interrupt flag and timer32bit flag.
 * Taking into account the given frequency, the function calculates the PR value and the prescaler,
 * storing these values inside the TimerConfig struct.
 *
 * @param targetFreq: Target frequency given by the user.
 * @param timer32bit: Timer32bit flag: 1 -> 32 bit Timer; 0 -> 16 bit Timer.
 */
void ConfigTimer2(uint32_t targetFreq, uint8_t timer32bit);

/**
 * Timer 3 Config function
 *
 * This function configures the Timer 3 with the given target frequency and interrupt flag.
 * Taking into account the given frequency, the function calculates the PR value and the prescaler,
 * storing these values inside the TimerConfig struct.
 *
 * @param targetFreq: Target frequency given by the user.
 */
void ConfigTimer3(uint32_t targetFreq);

/**
 * Timer 4 Config function
 *
 * This function configures the Timer 4 with the given target frequency, interrupt flag and timer32bit flag.
 * Taking into account the given frequency, the function calculates the PR value and the prescaler,
 * storing these values inside the TimerConfig struct.
 *
 * @param targetFreq: Target frequency given by the user.
 * @param timer32bit: Timer32bit flag: 1 -> 32 bit Timer; 0 -> 16 bit Timer.
 */
void ConfigTimer4(uint32_t targetFreq, uint8_t timer32bit);

/**
 * Timer 5 Config function
 *
 * This function configures the Timer 5 with the given target frequency and interrupt flag.
 * Taking into account the given frequency, the function calculates the PR value and the prescaler,
 * storing these values inside the TimerConfig struct.
 *
 * @param targetFreq: Target frequency given by the user.
 */
void ConfigTimer5(uint32_t targetFreq);

/**
 * Start Timer 1 function
 *
 * This function starts the Timer 1.
 */
void StartTimer1();

/**
 * Start Timer 2 function
 *
 * This function starts the Timer 2.
 */
void StartTimer2();

/**
 * Start Timer 3 function
 *
 * This function starts the Timer 3.
 */
void StartTimer3();

/**
 * Start Timer 4 function
 *
 * This function starts the Timer 4.
 */
void StartTimer4();

/**
 * Start Timer 5 function
 *
 * This function starts the Timer 5.
 */
void StartTimer5();


/**
 * Stop Timer 1 function
 *
 * This function stops the Timer 1.
 */
void StopTimer1();

/**
 * Stop Timer 2 function
 *
 * This function stops the Timer 2.
 */
void StopTimer2();

/**
 * Stop Timer 3 function
 *
 * This function stops the Timer 3.
 */
void StopTimer3();

/**
 * Stop Timer 4 function
 *
 * This function stops the Timer 4.
 */
void StopTimer4();

/**
 * Stop Timer 5 function
 *
 * This function stops the Timer 5.
 */
void StopTimer5();


/**
 * Get Timer 1 interrupt flag function
 *
 * This function returns the Timer 1 interrupt flag.
 *
 * @return Timer 1 interrupt flag.
 */
uint8_t GetIntFlagTimer1();

/**
 * Get Timer 2 interrupt flag function
 *
 * This function returns the Timer 2 interrupt flag.
 *
 * @return Timer 2 interrupt flag.
 */
uint8_t GetIntFlagTimer2();

/**
 * Get Timer 3 interrupt flag function
 *
 * This function returns the Timer 3 interrupt flag.
 *
 * @return Timer 3 interrupt flag.
 */
uint8_t GetIntFlagTimer3();

/**
 * Get Timer 4 interrupt flag function
 *
 * This function returns the Timer 4 interrupt flag.
 *
 * @return Timer 4 interrupt flag.
 */
uint8_t GetIntFlagTimer4();

/**
 * Get Timer 5 interrupt flag function
 *
 * This function returns the Timer 5 interrupt flag.
 *
 * @return Timer 5 interrupt flag.
 */
uint8_t GetIntFlagTimer5();


/**
 * Clear Timer 1 interrupt flag function
 *
 * This function clears the Timer 1 interrupt flag.
 */
void ClearIntFlagTimer1();

/**
 * Clear Timer 2 interrupt flag function
 *
 * This function clears the Timer 2 interrupt flag.
 */
void ClearIntFlagTimer2();

/**
 * Clear Timer 3 interrupt flag function
 *
 * This function clears the Timer 3 interrupt flag.
 */
void ClearIntFlagTimer3();

/**
 * Clear Timer 4 interrupt flag function
 *
 * This function clears the Timer 4 interrupt flag.
 */
void ClearIntFlagTimer4();

/**
 * Clear Timer 5 interrupt flag function
 *
 * This function clears the Timer 5 interrupt flag.
 */
void ClearIntFlagTimer5();


/**
 * Get Timer 1 Config function
 *
 * This function returns the Timer 1 Config.
 *
 * @return Timer 1 Config struct.
 */
struct TimerConfig GetTimer1Config();

/**
 * Get Timer 2 Config function
 *
 * This function returns the Timer 2 Config.
 *
 * @return Timer 2 Config struct.
 */
struct TimerConfig GetTimer2Config();

/**
 * Get Timer 3 Config function
 *
 * This function returns the Timer 3 Config.
 *
 * @return Timer 3 Config struct.
 */
struct TimerConfig GetTimer3Config();

/**
 * Get Timer 4 Config function
 *
 * This function returns the Timer 4 Config.
 *
 * @return Timer 4 Config struct.
 */
struct TimerConfig GetTimer4Config();

/**
 * Get Timer 5 Config function
 *
 * This function returns the Timer 5 Config.
 *
 * @return Timer 5 Config struct.
 */
struct TimerConfig GetTimer5Config();


#endif // TIMER_H