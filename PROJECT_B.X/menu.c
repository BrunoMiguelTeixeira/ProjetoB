#include "menu.h"
#include "sys_config.h"
#include "uart.h"
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>

void DefaultMenu(uint16_t temp, uint8_t minTemp, uint8_t maxTemp){
    PutString("\e[1;1H\e[2J");  //Clear screen  
    PutString("\e[33m");        // Make the letters yellow
    
    PutString("Temperature: ");
    PutInt(temp);
    PutString("  ºC");
    
    PutString("\t| Min: ");
    PutInt(minTemp);
    PutString("\t| Max: ");
    PutInt(maxTemp);
    PutStringn("\e[0m");         // Reset to default color (white)
}

void Menu(uint8_t option, int value, float kp, float ki){
    switch(option){
        case 0:
            PutStringn("---------MENU--------");
            PutStringn("1- Input Desired Temp");
            PutStringn("2- Show PI weights");
            PutStringn("3- Reset Read Values");
            PutString("Choice: ");
            PutInt(value);
            PutStringn(" ");
            break;
        case 1:
            PutString("Desired Temperature: ");
            PutInt(value);
            PutStringn(" ");
            break;
        case 2:
            PutStringn("PI Weights:");
            PutString("Kp: ");
            PutFloat(kp,3);
            PutStringn(" ");
            PutString("Ki: ");
            PutFloat(ki,3);
            PutStringn(" ");
            break;
        case 3:
            PutStringn("Read values reseted; Press Enter.");
            PutStringn(" ");
            break;
        default:
            PutStringn("Invalid Option; Press Enter.");
            break;
    };
}