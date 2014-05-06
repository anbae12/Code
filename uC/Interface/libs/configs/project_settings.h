/*****************************************************************************
* Odense University College of Engineering
* Embedded Programming (EMP)
*
* MODULENAME.: string_queue
*
* PROJECT....: My own
*
* DESCRIPTION:
* puts a c string into a queue as individual characters or turns characters from a queue into a string
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140407  NI    created
*
*****************************************************************************/

#ifndef PRO_SETTINGS_H
  #define PRO_SETTINGS_H

/***************************** Include files *******************************/
#include "semaphore_project.h"
#include "tmodel.h"
/*****************************    Defines    *******************************/

#define INTERFACE_TASK_DELAY_MS 10
#define UART_QUEUE_LEN 10
#define QUEUE_LEN 10
#define COORDINATE_LEN 11 //eg 3*3 + 2 = 11

#define MAX_INPUT_CHAR 20
#define SCREEN_RES     16 //number of lines in putty (44 for fullscreen 16 for default size)


#define GSS_IDLE        1
#define GSS_ACC_ID      2
#define GSS_ACC_PIN     3
#define GSS_CASH        4
#define GSS_FILLING     5
#define GSS_UPDATE      6


#define ASS_ACCOUNT   1
#define ASS_USER_PETER    2
#define ASS_USER_HANS     3
#define ASS_USER_SANDRA   4

#define ASS_RESPONSE_ACC_TRUE   1
#define ASS_RESPONSE_ACC_FALSE  2
#define ASS_RESPONSE_PIN_TRUE   3
#define ASS_RESPONSE_PIN_FALSE  4


//used in uart_communication
#define MAX_UART_IN    20
#define QUERY_A         1
#define QUERY_B         2
#define QUERY_C         3
#define PRICE_A         4
#define PRICE_B         5
#define PRICE_C         6
#define VALUE           7
#define UNKNOWN_INPUT   8



/*****************************   Constants   *******************************/
/*****************************   Functions   *******************************/
/****************************** End Of Module *******************************/

#endif
