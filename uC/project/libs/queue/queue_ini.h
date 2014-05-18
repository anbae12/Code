/*****************************************************************************
* Odense University College of Engineering
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
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
/*****************************    Defines    *******************************/

#define ENC_QUEUE_LENGTH                   8
#define ENC_QUEUE_DATA_SIZE                2
/*****************************   Constants   *******************************/

extern xQueueHandle enc_queue[2];
extern xQueueHandle pos_ctrl_queue;

extern xSemaphoreHandle position_ctrl_mutex;
extern xSemaphoreHandle target_var_mutex;

//set pwm
extern xSemaphoreHandle interface_pwm_mutex;
extern xSemaphoreHandle target_pwm_mutex;

//control the control_task
extern xSemaphoreHandle interface_to_control_mutex;
extern xSemaphoreHandle interface_pwm_mutex;

//log task
extern xSemaphoreHandle interface_log_mutex;
extern xQueueHandle log_status_queue;


//task on/off
extern portBASE_TYPE we_use_read_task;
/*****************************   Variables   *******************************/
extern void init_sem_and_queues( void );
extern void add_to_enc_queue(INT8U queue_id, INT16U data);
/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

