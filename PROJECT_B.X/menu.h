#ifndef __MENU_H__
#define __MENU_H__
/*
 * File: menu.h
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

#include <xc.h>

/**
* Function: 	DefaultMenu()\n
* Precondition: \n
* Input: 		Instantaneous temperature,minimum temperature recorded, maximum temperature recorded\n
* Returns:      none\n
* Overview:     Prints always the required values to be shown\n
*/
void DefaultMenu(uint16_t temp, uint8_t minTemp, uint8_t maxTemp);


/**
* Function: 	Menu()\n
* Precondition: \n
* Input: 		Menu location,read input value, Pi Kp weight, Pi Ki weight \n
* Returns:      none\n
* Overview:     Prints accordingly to menu location at the time, using value to display
*               current user input. Ki and Kp only used to be displayed  \n
*               
*/
void Menu(uint8_t option, int value, float kp, float ki);


#endif