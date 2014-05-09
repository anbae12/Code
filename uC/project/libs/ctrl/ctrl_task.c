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
#include "controller.h"
/*****************************    Defines    *******************************/
#define PI 3.14159265359
#define TICKS_PER_DEGREE 1080/360 //I know this is 3 but this is more descriptive
#define CTRL_DEBUG 1
#define CTRL_TASK_FREQUENCY 200

void ctrl_debug(motor_pos target)
{
  if ( CTRL_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) target.positionA;
    PRINTF("target phi: %d\n",conv);
    conv = (INT16U) target.positionB;
    PRINTF("target theta: %d\n",conv);
  }
}

void ctrl_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  FP32 motor_position_A;
  FP32 motor_position_B;
  
  INT16U motor_pwm_A;
  INT16U motor_pwm_B;

  INT16U target_pos_format_a;

  pwm_duty_cycle_type next_pwm;

  motor_pos current_pos;
  motor_pos target_pos;

  //for timing
  portTickType last_wake_time;
  last_wake_time = xTaskGetTickCount();

  while(1)
  {
    
    target_pos = get_target_position();
    current_pos = spi_read_encoders();
#if 0
    INT16U temp;
    temp = (INT16U) current_pos.positionA;
    PRINTF("Position A %u\n",temp); //tilt
    temp = (INT16U) current_pos.positionB;
    PRINTF("Position B %u\n",temp);
#endif
    //motor_pwm_A = pan_controller(target_pos);
    //motor_pwm_B = tilt_controller(current_pos);
    //next_pwm = control_loop(current_pos , target_pos);

    next_pwm = test_controller(target_pos, current_pos);

    spi_send_pwm(next_pwm);

    vTaskDelayUntil(&last_wake_time, MILLI_SEC(CTRL_TASK_FREQUENCY));
  }
}

motor_pos get_target_position()
{
  static motor_pos target;
  coordinate_type coord;

  if( xSemaphoreTake(target_var_sem, 1) )
  {
    coord = target_var;
    xSemaphoreGive(target_var_sem);
  }

  target.positionA = atan((sqrt(pow(coord.x,2) + pow(coord.y,2)))/coord.z) * 180/PI; //phi
  target.positionB = atan(coord.y/coord.x) * 180/PI; //theta

  ctrl_debug(target);

  return target;
}

