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

extern void ctrl_task();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    
 *              Reads ctrl bit and chose state
 *              Receive target position
 *              Convert target position
 *              Calculate PWM
 *              Send PWM to SPI
 *              Receives position from SPI
 *              Put position into status
 ****************************************************************************/

/****************************** End Of Module *******************************/
