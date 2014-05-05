/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: uart.h
*
* PROJECT....: EMP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140401  NI    added uart_string_put which sends a string
*
*****************************************************************************/

#ifndef _UART_H
  #define _UART_H

/***************************** Include files *******************************/

#include "inc/emp_type.h"
#include "configs/uartcnf.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

extern void uart0_init(void);
/******************************************************************************
* This function configures the UART0 for receiving and transmitting operation
* with the specified baud rate set in uartcnf.h file.
* Data format: 8 databit, no parity bit, one stop bit and FIFO enabled
******************************************************************************/


extern BOOLEAN uart_char_put_non_blocking(INT8S data);
/******************************************************************************
*  Sends a character to the specified port.
*
* This function writes the character data to the transmit FIFO for UART0
* This function does not block, so if there is no space available,
* then a FALSE is returned and the application must retry the function later.
*
*
* Returns TRUE if the character was successfully placed in the
* transmit FIFO or FALSE if there was no space available in the transmit FIFO.
*****************************************************************************/


extern BOOLEAN uart_char_avail(void);
/*****************************************************************************
* Determines if there are any characters in the receive FIFO.
*
* This function returns a flag indicating whether or not there is data
* available in the receive FIFO.
*
* Returns TRUE if there is data in the receive FIFO or
* FALSE if there is no data in the receive FIFO.
*****************************************************************************/


extern INT8S uart_char_get_non_blocking(void);
/*****************************************************************************
* Receives a character from UART0
*
* This function gets a character from the receive FIFO for the UART0
*
* Returns the character read from the UART0 port
* A -1 is returned if there are no characters present in the receive FIFO.
* The uart_chars_avail() function should be called before
* attempting to call this function.
*****************************************************************************/

extern void uart_string_put(INT8U* input);
extern void uart_number(INT16U input);
/*****************************************************************************
* Receives a character array from as input
*
* It sends each character to uart_char_put_non_blocking. 
* No return because why the fuck would I care???
*****************************************************************************/


/****************************** End Of Module *******************************/
#endif






