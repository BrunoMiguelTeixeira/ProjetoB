#include "menu.h"
#include "sys_config.h"
#include "uart.h"
#include <sys/attribs.h>
#include <xc.h>
#include <stdio.h>

void DefaultMenu(uint16_t temp){
    PutString("\e[1;1H\e[2J");  //Clear screen  
    PutString("\e[33m");        // Make the letters yellow
    
    PutString("Temperature: ");
    PutInt(temp);
    PutStringn(" ºC");

    PutString("\e[0m");         // Reset to default color (white)
}

/*
uint8_t SelectTemp(){
    PutString("Desired Temperature: ");
    while(GetChar() != '/n'){   
        val = GetInteger();
        PutInt(val);
    }
}*/

void Menu(uint8_t option, int value){
    switch(option){
        case 0:
            PutStringn("-----MENU----");
            PutStringn("1- Input Desired Temp");
            PutStringn("2- Show PID weights");
            PutStringn("3- Reset Read Values");
            PutString("Choice: ");
            PutInt(value);
            break;
        case 1:
            PutString("Desired Temperature: ");
            PutInt(value);
            break;
        case 2:
            PutStringn("PID Weights:");
            PutStringn("\tKp: ");
            //PutInt(Kp);
            PutStringn("\tKi: ");
            //PutInt(Ki);
            PutStringn("\tKd: ");
            //PutInt(Kd);
            break;
        case 3:
            PutStringn("Read values reseted...");
            break;
        default:
            PutStringn("Invalid Option; Press Enter.");
            break;
    };
}