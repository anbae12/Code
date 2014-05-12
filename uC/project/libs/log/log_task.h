/*****************************************************************************
 * Nikolaj
 * MODULENAME:  log_task.h
 * DESCRIPTION:
 ****************************************************************************/

#pragma once

/***************************** Include files *******************************/
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "inc/emp_type.h"

/*****************************    Defines    *******************************/
#define MAX_LOG_ENTRIES 150

/******************************** Variables *********************************/
typedef struct log_file_type {
  INT16U current_pos_A;
  INT16U current_pos_B;
  INT16U target_pos_A;
  INT16U target_pos_B;
  INT16U pwm_motor_A;
  INT16U pwm_motor_B;
} log_file_type;

extern log_file_type log_global[MAX_LOG_ENTRIES];
/*****************************   Constants   *******************************/
/*****************************   Functions   *******************************/
extern void log_task(void *pvParameters);
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    
 *              Reads the log_status_queue for input
 *              Waits for the interface_log_sem is given from the interface.
 *              When the semaphore is given the log task starts to print.
 *
 ****************************************************************************/
extern void print_log(log_file_type log[MAX_LOG_ENTRIES]);
/*****************************************************************************
 * Prints every entry as a comma separated list.
 ****************************************************************************/
extern void reset_log(log_file_type log[MAX_LOG_ENTRIES]);
/*****************************************************************************
 * sets every entry to 0
 ****************************************************************************/
/****************************** End Of Module *******************************/
