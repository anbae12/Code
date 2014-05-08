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
#include "read_pos/read_pos.h"
#include "ctrl_task.h"
#include <math.h>

/*****************************    Defines    *******************************/
#define PI 3.14159265359
#define TICKS_PER_DEGREE 1080/360 //I know this is 3 but this is more descriptive
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

  INT16U target_pos_format_a;
/*
  position_type_struct current_pos_format_a;
  pwm_type_struct next_pwm;
*/
  motor_pos current_pos;
  motor_pos target_pos;

  while(1)
  {
    
    target_pos = get_target_position(target_pos_queue);
/*    current_pos = spi_get_position();
    next_pwm = control_loop(current_pos , target_pos);
    spi_send(next_pwm);
    */
  }
}

motor_pos get_target_position(xQueueHandle queue_name)
{
  static motor_pos target;
  coordinate_type coord;
  FP32 phi;
  FP32 theta;

  if( uxQueueMessagesWaiting(queue_name) > 0 )
      {
        xQueueReceive(queue_name, &coord , 0 );

        phi = atan((sqrt(pow(coord.x,2) + pow(coord.y,2)))/coord.z) * 180/PI;
        theta = atan(coord.y/coord.x) * 180/PI;

        target.positionA = phi * TICKS_PER_DEGREE;
        target.positionB = theta * TICKS_PER_DEGREE;
      }
  return target;
}

