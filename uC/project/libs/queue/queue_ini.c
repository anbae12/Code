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

#include "read_pos/read_pos.h"
#include "log/log_task.h"

#include "queue_ini.h"



/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

//xQueueHandle enc_queue[2];
//xQueueHandle pos_ctrl_queue;

xSemaphoreHandle position_ctrl_sem;
xSemaphoreHandle target_var_sem;

xSemaphoreHandle interface_pwm_sem;
xSemaphoreHandle target_pwm_sem;

xSemaphoreHandle interface_to_control_sem;
xSemaphoreHandle interface_pwm_sem;

xSemaphoreHandle interface_log_sem;
xQueueHandle log_status_queue;


/*****************************   Variables   *******************************/
void init_sem_and_queues( void )
{
  //Mutexes
  position_ctrl_sem = xSemaphoreCreateMutex();
  target_var_sem = xSemaphoreCreateMutex();

  interface_pwm_sem = xSemaphoreCreateMutex();
  target_pwm_sem = xSemaphoreCreateMutex();

  interface_to_control_sem = xSemaphoreCreateMutex();
  interface_pwm_sem = xSemaphoreCreateMutex();

  interface_log_sem = xSemaphoreCreateMutex();
  log_status_queue = xQueueCreate(15, sizeof(log_file_type) );

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

