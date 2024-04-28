#ifndef THERMO_H
#define THERMO_H

/*
 * File: thermo_k.h
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
// Array for all the millivolt values for each temp (1ºC resolution)
// Type K Thermocouple, from -10ºC to 90ºC

extern int thermo_k[151];   //*< Array for all the millivolt values for each temp (1ºC resolution), directly from the datasheet.
extern float therm_amp[151];    //*< Array based on thermo_k, but with the gains and offset applied.

#endif