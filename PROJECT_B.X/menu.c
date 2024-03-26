#include "menu.h"
#include "sys_config.h"
#include "uart.h"
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>

void DefaultMenu(uint8_t temp){
    printf("\e[1;1H\e[2J");                 //Clear screen
    PutString("Temperature: ");
    PutInt(temp);
    PutStringn(" ºC");
}

void Menu(uint8_t option,int Value){
    switch(option){
        case 0:
            PutStringn("-----MENU----");
            PutStringn("1-Input Desired Temp");
            PutStringn("2-Show PID weights");  
            PutString("Choice: ");
            PutInt(Value);
            break;
        case 1:
            PutString("Desired Temperature: ");
            PutInt(Value);
            break;
        case 2:
            break;
        default:
        break;
    };
}