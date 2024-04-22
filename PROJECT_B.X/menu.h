#ifndef __MENU_H__
#define __MENU_H__

#include <xc.h>

void DefaultMenu(uint16_t temp, uint8_t minTemp, uint8_t maxTemp);

void Menu(uint8_t option, int value, float kp, float ki);


#endif