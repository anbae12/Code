/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: FreeRTOS adaptation.
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140408  NI   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "configs/project_settings.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "configs/project_settings.h"
#include "SPI/spi.h"
#include "queue/queue_ini.h"

/*****************************    Defines    *******************************/

void ctrl_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  INT16U motor_position_A;
  INT16U motor_position_B;
  
  INT16U motor_pwm_A;
  INT16U motor_pwm_B;
  //position_type current_pos;
  while(1)
  {
    //if( uxQueueMessagesWaiting(target_position_something_queue)  )
    //current_pos = spi_receive();
    //next_pwm = control_loop(current_pos);
    //spi_send(next_pwm);
    
    // Read target position. 
    // Read current position. (SPI)
    // Control loop. 
    // Set_pwm.
    // motor_pwm_A = PWM_MASK & calculated_speed_A 
    // motor_pwm_B = PWM_MASK & calculated_speed_B 
    // spi_buffer_push(motor_pwm_A);
    // spi_buffer_push(motor_pwm_B);
  }
}
