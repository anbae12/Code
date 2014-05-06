/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME:  uart
 * DESCRIPTION:
 ****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include "inc/emp_type.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"

/*****************************    Defines    *******************************/
#define UART_QUEUE_LEN  50

/*****************************   Constants   *******************************/

/******************************** Variables *********************************/
//---------------- Tasks ------------------
extern xTaskHandle      uart_send_task_handle;          //Create elsewhere.
extern xTaskHandle      uart_receive_task_handle;       //Create elsewhere.

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



INT8S uart_char_get_blocking();
/*****************************************************************************
 * Input:       -
 * Output:      A char from UART.
 * Function:    Blocks until a char is received.
 ****************************************************************************/

void uart_send_task(void *pvParameters);
/*****************************************************************************
 * Input:       pvParameters:   Unused.
 * Output:      -
 * Function:    Empties uart_send_queue and sends the characters through
 *              UART.
 ****************************************************************************/

void uart_receive_task(void *pvParameters);
/*****************************************************************************
 * Input:       pvParameters:   Unused.
 * Output:      -
 * Function:    Takes characters from UART and places them in
 *              uart_receive_queue.
 ****************************************************************************/

void uart_push_string(char *string_to_push);
/*****************************************************************************
 * Input:       string_to_push: A string to send, null-terminated.
 * Output:      -
 * Function:    Places the string in the uart_send_queue.
 *              Uses the uart_outgoing_mutex to protect the string.
 ****************************************************************************/

void uart_push_char(INT8U char_to_push);
/*****************************************************************************
 * Input:       char_to_push: A char to send.
 * Output:      -
 * Function:    Places the char in the uart_send_queue.
 *              Uses the uart_outgoing_mutex, so the char doesn't get sent
 *              while a string is in the process of being sent.
 ****************************************************************************/

void uart_direct_push_char(INT8U char_to_push);
/*****************************************************************************
 * Input:       char_to_push: A char to send.
 * Output:      -
 * Function:    Places the char in the uart_send_queue,
 *              regardless of uart_outgoing_mutex status.
 ****************************************************************************/

BOOLEAN uart_pop_string_echo(char *string, INT8U string_length, BOOLEAN echo);
/*****************************************************************************
 * Input:       string:                 string to write popped string to.
 *              string_length:  Maximal length of popped string
 *                                      (length of first argument 'string').
 *              echo:   Boolean indicating whether or not each char received
 *                      through UART should be echoed back ASAP.
 * Output:      FALSE if nothing is in the receive queue - no string is being
 *              entered.
 *              TRUE if a string was succesfully popped.
 * Function:    If there's something in the receive queue, a string is placed
 *              in 'string'. The reception stops when a carriage return is
 *              encountered or when string_max_length has been reached.
 *              Blocks while waiting for chars.
 ****************************************************************************/

BOOLEAN uart_pop_string(char *string, INT8U string_max_length);
/*****************************************************************************
 * Function:    See uart_pop_string_echo().
 *              Without echoing back through UART, pops a string.
 ****************************************************************************/

void take_uart_output_permission();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    Takes the UART outgoing mutex.
 ****************************************************************************/

void give_uart_output_permission();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    Gives the UART outgoing mutex.
 ****************************************************************************/

void init_uart_receive_task();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:
 ****************************************************************************/

void init_uart_send_task();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:
 ****************************************************************************/



/****************************** End Of Module *******************************/
