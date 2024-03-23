#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "uart.h"

/********************************************************************
* Function: 	UartInit()
* Precondition: 
* Input: 		PB Clock and baudrate
* Returns:      UART_SUCCESS if Ok.
*               UARTX_XXX error codes in case of failure (see uart.h)
* Side Effects:	Takes control of U1A TX and RX pins
* Overview:     Initializes UART.
*		
* Note:		 	Only supports 40MHz PBCLOCK, UART1A and 
*               {9600/115200},8,n,1 configuration
* 
********************************************************************/	
int UartInit(uint64_t pbclock, uint32_t br)
{
   if(pbclock != 40000000L) 
       return UART_PBCLOCK_NOT_SUP; // Todo: add support to common pbclock values.
       
   // In the following are used the table values provided in the datasheet
   //PIC32MX Family Reference Manual, DS61107E-pages 21-14 and following
 	switch(br) {
        case 115200:
            U1ABRG=21; 
            U1AMODEbits.BRGH = 0;
            break;
            
        case 9600:
            U1ABRG=259; 
            U1AMODEbits.BRGH = 0;
            break;
        default:
            return UART_BR_NOT_SUP; // Baudrate not supported
    }
    
    // Common configuration settings
    U1AMODEbits.SIDL=0; // Continue operation in idle mode 
    U1AMODEbits.IREN=0; //Disable Irda
    U1AMODEbits.UEN=0;  //Only use TX and RX pints. No HW flow control 
    U1AMODEbits.WAKE=0;  //Wake -up on start bit disabled
    U1AMODEbits.LPBACK=0;  //No loopback  
    U1AMODEbits.ABAUD=0;  //No autobaud
    U1AMODEbits.RXINV=0;  //Idle logic value is 1
    U1AMODEbits.PDSEL=0;  //8 bit data, no parity
    U1AMODEbits.STSEL=0;  //1 stop bit
    U1STAbits.ADM_EN = 0; // No automatic address detection
    U1STAbits.UTXISEL = 0; // Interrupt when TX buffer has at least 1 empty position
    U1STAbits.UTXINV = 0; // Idle logic value is 1
    
    // Configuration done. Enable.   
    U1AMODEbits.ON = 1;  
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    return UART_SUCCESS;
    
}	


/********************************************************************
* Function: 	UartClose()
* Precondition: 
* Input: 		None
* Output:		None.
* Side Effects:	None.
* Overview:     Closes UART connection.
* Note:		 	No function currently
********************************************************************/	
int UartClose(void)
{
	//TODO: do some closing operation if required.	
    return UART_SUCCESS;
}	


/********************************************************************
* Function: 	GetChar()
* Precondition: UART initialized
* Input: 		None
* Output:		UART_SUCESS: If there is some data
 *              UART_FAIL: if there is no data.
* Side Effects:	None.
* Overview:     Gets the data from UART RX FIFO.
* Note:		 	None.
********************************************************************/
int GetChar(uint8_t *byte)
{
	char dummy;

	if(U1STAbits.OERR ||U1STAbits.FERR || U1STAbits.PERR) // receive errors?
	{
		dummy = U1RXREG; 			// dummy read to clear FERR/PERR
		U1STAbits.OERR = 0;			// clear OERR to keep receiving
	}

	if(U1STAbits.URXDA)
	{
		*byte = U1ARXREG;		        // get data from UART RX FIFO
		return UART_SUCCESS;
	}
	
	return UART_FAIL;

}


/********************************************************************
* Function: 	PutChar()
* Precondition: 
* Input: 		None
* Output:		None
* Side Effects:	None.
* Overview:     Puts the data into UART tx reg for transmission.
* Note:		 	None.
********************************************************************/
void PutChar(uint8_t txChar)
{
    while(U1STAbits.UTXBF); // wait for TX buffer to be empty
    U1ATXREG = txChar;
}

void PutString(char *Char)
{
    while(*Char != 0){
        PutChar(*Char);
        Char++;
    }
}

void PutStringn(char *Char){
    PutString(Char);
    PutChar('\r');
    PutChar('\n');
}
void nPutString(char *Char){
    PutChar('\r');
    PutChar('\n');
    PutString(Char);
}


void PutInt(int Integer){
        if (Integer == 0) {
        PutChar('0');
        return;
    }

    // Handle negative numbers
    if (Integer < 0) {
        PutChar('-');
        Integer = -Integer;
    }

    // Recursively print digits
    if (Integer / 10 != 0) {
        PutInt(Integer / 10);
    }

    // Print the last digit
    PutChar(Integer % 10 + '0');
}
void PutIntn(int Integer){
    PutInt(Integer);
    PutChar("\r");
    PutChar("\n");
}
/*******************************************************************
 * Function: PutFloat()
 * Precondition:
 * Input:       Float
 * Output:      None
 * Side Effects:None
 * Overview:    Splits Float into two integers to be printed has a char and into UART tx reg for transmission.
 * Note:        None
 *******************************************************************/
void PutFloat(float Float,uint8_t val){
    if(val<1){
        PutStringn("ERR FLOAT SIZE");
    }
    PutInt((int) Float);
    Float=(Float-(int)Float)*10*val;
    PutChar('.');
    PutInt((int)Float);
}

uint8_t GetInteger(void){
    uint8_t dummy;
    uint8_t value;
    if(GetChar(&dummy) == UART_SUCCESS){        
        if(dummy==13){//Enter pressed
            return 10;
        }
        if(dummy!=194 & 10>dummy-'0'>-1){
            value= dummy-'0';
            return value;
        }
    }
}

/***************************************End Of File*************************************/
