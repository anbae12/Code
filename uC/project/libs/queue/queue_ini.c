/*****************************************************************************
 * Odense University College of Enginerring
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: queue_ini.c
 *
 * PROJECT....: Pan and tilt
 *
 * DESCRIPTION:
 * This module is used to initialize freeRTOS queues.
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 050128  KA    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "inc/binary.h"

#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"


#include "ctrl/ctrl_task.h"

#include "queue_ini.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

//xQueueHandle enc_queue[2];
//xQueueHandle pos_ctrl_queue;

xSemaphoreHandle position_ctrl_mutex;
xSemaphoreHandle target_var_mutex;

xSemaphoreHandle interface_pwm_mutex;
xSemaphoreHandle target_pwm_mutex;

xSemaphoreHandle interface_to_control_mutex;
xSemaphoreHandle interface_pwm_mutex;

xSemaphoreHandle interface_log_mutex;
xQueueHandle log_status_queue;
portBASE_TYPE we_use_read_task = pdFALSE;


/*****************************   Variables   *******************************/
void init_sem_and_queues( void )
{
  //Mutexes
  position_ctrl_mutex = xSemaphoreCreateMutex();
  target_var_mutex = xSemaphoreCreateMutex();

  interface_pwm_mutex = xSemaphoreCreateMutex();
  target_pwm_mutex = xSemaphoreCreateMutex();

  interface_to_control_mutex = xSemaphoreCreateMutex();
  interface_pwm_mutex = xSemaphoreCreateMutex();

  interface_log_mutex = xSemaphoreCreateMutex();
  //log_status_queue = xQueueCreate(LOG_QUEUE_SIZE, sizeof(log_file_type) );
  interface_to_control_queue = xQueueCreate(2, sizeof(message_user_interface_type) );

}
void add_to_enc_queue(INT8U queue_id, INT16U data)
{
  if ( enc_queue[queue_id] != 0 )
  {
    if (xQueueSendToFront(enc_queue[queue_id], &data, 100))
    {
      // ERROR!
    }
  }
}


/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

