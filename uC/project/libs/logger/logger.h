/*****************************************************************************
 *
 * MODULENAME: logger
 * DESCRIPTION:
 ****************************************************************************/
#pragma once
/****************************** Include files *******************************/
#include "configs/project_settings.h"
#include "inc/emp_type.h"
/********************************* Defines **********************************/

typedef struct closedloop_entry {
  INT16U current_posA;
  INT16U current_posB;
  INT16U setpointA;
  INT16U setpointB;
  INT16S pwmA;
  INT16S pwmB;
} log_entry;

/******************************** Constants *********************************/

/******************************** Variables *********************************/

/******************************** Functions *********************************/

void init_logger_presched();

void logger_task(void *pvParameters);

void log_entry_register_closedloop(log_entry *the_entry);

void log_flush(BOOLEAN print);

void display_log_format(void);

/****************************** END OF MODULE *******************************/
