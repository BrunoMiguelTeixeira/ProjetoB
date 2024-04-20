/*--- SYSTEM CLOCKS ---*/
#define SYSCLK 80000000L        /**< System clock frequency, in Hz          */
#define PBCLOCK 40000000L       /**< Peripheral Bus clock frequency, in Hz  */

/* Analog to Digital section: */

#define ADC_CHAN_THERMIC 14       /**< ADC channel for thermistor (0 to 15)  */
#define ADC_CHAN_THERMO 15        /**< ADC channel for thermocouple (0 to 15) */
#define SAMPL_FREQ_HZ 10       /**< Sampling frequency, in Hz              */

/* PWM section: */
#define PWM_OC_CHAN 1           /**< PWM output compare channel (1 to 5)    */
#define PWM_FREQ_HZ 2000        /**< PWM frequency, in Hz                   */

/* Temperature sensor section: */
#define MIN_DESIRED_TEMP 50     /**< Minimum desired temperature, in Celsius */
#define MAX_DESIRED_TEMP 90     /**< Maximum desired temperature, in Celsius */


// Sinal quadrado: Dominio freq -> DC maybe; Freq do sinal; Harmonicas;
// Nada de fotos nos resultados; Dar medidas juntamente com fotos ou até gráficos;
// Resultados e Analise de Resultados, apresentar valores, medidas, gráficos, etc;
// Testar o sistema em varias condições (Ex primeiro trabalho: Fzr para sinais sinuosidades, Quadrados...);
// Justificar o porque de usar aquele metodo, ou aquele circuito
// SI -> Sistema Internacional de Unidades
// Adicionar bibliografia para componentes, datasheets, etc;