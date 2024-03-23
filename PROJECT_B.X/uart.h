#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

// Define return codes
#define UART_SUCCESS 0
#define UART_FAIL -1;
#define UART_BR_NOT_SUP -2
#define UART_PBCLOCK_NOT_SUP -3

// Define prototypes (public interface)
int UartInit(uint64_t pbclock, uint32_t br);
int UartClose(void);
int GetChar(uint8_t *byte);
void PutChar(uint8_t txChar);

/**
 * Function: PutInt()\n
 * Precondition:\n
 * Input:       Integer\n
 * Output:      None\n
 * Side Effects:None\n
 * Overview:    Puts the integer has a char and into UART tx reg for transmission\n
 * Note:        None\n
 */
void PutInt(int Integer);

void PutIntn(int Integer);
/**
 * Function: PutFloat()\n
 * Precondition:\n
 * Input: Float\n
 * Output: None\n
 * Side Effects: None\n
 * Overview: Puts the float has a char and into UART tx reg for transmission\n
 * Note: None\n
 */
void PutFloat(float Float,uint8_t val);

/**
* Function: PutString()\n
* Precondition: \n
* Input: *Char\n
* Output: None\n
* Side Effects:	None\n
* Overview: Breaks the string into Char and puts it into UART tx reg for transmission\n
* Note: None\n
*/
void PutString(char *Char);


/**
* Function: PutStringn()\n
* Precondition: \n
* Input: *Char\n
* Output: None\n
* Side Effects:	None\n
* Overview: Breaks the string into Char,add '\r' '\ n' at the end and puts it into UART tx reg for transmission\n
* Note: None\n
*/
void PutStringn(char *Char);


/**
* Function: nPutString()\n
* Precondition: \n
* Input: *Char \n
* Output: None\n
* Side Effects:	None\n
* Overview: Breaks the string into Char,add '\r' '\ n' at the beginning and puts it into UART tx reg for transmission\n
* Note: None\n
*/
void nPutString(char *Char);


/**
 * Function: GetInteger()\n
 * Precondition:\n
 * Input: None\n
 * Output: Unsigned integer\n
 * Side Effects: None\n
 * Overview: Reads user number inputs until Enter is pressed\n
 * Note: None\n
*/
uint8_t GetInteger(void);
#endif