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
#include "read_pwm/read_pwm.h"
#include "ctrl_task.h"
#include <math.h>
#include "controller.h"
#include "log/log_task.h"
/*****************************    Defines    *******************************/
#define PI 3.14159265359
#define TICKS_PER_DEGREE 1080/360 //I know this is 3 but this is more descriptive
#define CTRL_TASK_FREQUENCY 1000

/******************************** Variables *********************************/
INT8U interface_to_control_byte = 0b10000000;

void current_pos_debug(motor_pos target)
{
  if ( CTRL_CURRENT_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) target.positionA * TICKS_PER_DEGREE;
    PRINTF("current ticks: %d\t",conv);
    conv = (INT16U) target.positionB * TICKS_PER_DEGREE;
    PRINTF("current ticks: %d\n",conv);
  }
}

void target_pwm_debug(pwm_duty_cycle_type target)
{
  if ( TARGET_PWM_DEBUG )
  {
      INT16S conv;
      conv = (INT16S) target.motorA;
      PRINTF("target pwm A: %d\t",conv);
      conv = (INT16S) target.motorB;
      PRINTF("target pwm B: %d\n",conv);
    }
}

void target_pos_debug(motor_pos target)
{
  if ( CTRL_TARGET_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) target.positionA;
    PRINTF("target phi: %d\t",conv);
    conv = (INT16U) target.positionB;
    PRINTF("target theta: %d\n",conv);
  }
}

void pwm_spi_debug(pwm_duty_cycle_type target)
{
  if ( READ_PWM_DEBUG )
  {
    INT16S conv;
    conv = (INT16S) target.motorA;
    PRINTF("pwm A: %d\t",conv);
    conv = (INT16S) target.motorB;
    PRINTF("pwm B: %d\n",conv);
  }
}

INT8U interface_input(void)
{
  static INT8U control_state = CTRL_STOP_MODE;
  if( xSemaphoreTake(interface_to_control_sem, 0) )
  {
    if (1 & (interface_to_control_byte >> stop_bit_location ) )
    {
      control_state = CTRL_STOP_MODE;
    }
    else if (1 & (interface_to_control_byte >> pos_bit_location ) )
    {
      control_state = CTRL_POS_MODE;
    }
    else if (1 & (interface_to_control_byte >> pwm_bit_location ) )
    {
      control_state = CTRL_PWM_MODE;
    }
    xSemaphoreGive(interface_to_control_sem);
  }
  return control_state;
}



void ctrl_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  pwm_duty_cycle_type next_pwm;

  motor_pos current_pos;
  motor_pos target_pos;

  //for timing
  portTickType last_wake_time;

  last_wake_time = xTaskGetTickCount();

  INT8U control_state;
  while(1)
  {

    control_state = interface_input();
    current_pos = spi_read_encoders();
    switch(control_state)
    {
    case CTRL_STOP_MODE:
      next_pwm.motorA = 0;
      next_pwm.motorB = 0;
      break;
    case CTRL_POS_MODE:
      target_pos = get_target_position();
      next_pwm = test_controller(target_pos, current_pos);
      //next_pwm.motorA = pan_controller(target_pos, current_pos);
      //next_pwm.motorB = tilt_controller(target_pos, current_pos);
      break;
    case CTRL_PWM_MODE:
      next_pwm = get_target_pwm();
      break;
    default:
      break;
    }

    current_pos_debug(current_pos);

    pwm_spi_debug(next_pwm);

    spi_send_pwm(next_pwm);

    set_status_log(next_pwm, current_pos, target_pos);

    //_wait(MILLI_SEC(CTRL_TASK_FREQUENCY));
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

  target.positionA = 90- ( atan((sqrt(pow(coord.x,2) + pow(coord.y,2)))/coord.z) * 180/PI ); //phi
  target.positionB = ( atan(coord.y/coord.x) * 180/PI ); //theta

  if(target.positionA < 0)
  {
    target.positionA += 360;
  }
  if(target.positionB < 0)
  {
    target.positionB += 360;
  }
  target_pos_debug(target);

  return target;
}

pwm_duty_cycle_type get_target_pwm()
{
  static pwm_duty_cycle_type target;
  if( xSemaphoreTake(target_pwm_sem, 1) )
  {
    target = target_pwm;
    xSemaphoreGive(target_pwm_sem);
  }
  target_pwm_debug(target);
  return target;
}

void set_status_log(pwm_duty_cycle_type pwm, motor_pos current, motor_pos target)
{
  static log_file_type hans;

  hans.current_pos_A = (INT16U) current.positionA * TICKS_PER_DEGREE;
  hans.current_pos_B = (INT16U) current.positionB * TICKS_PER_DEGREE;
  hans.target_pos_A = (INT16U) target.positionA * TICKS_PER_DEGREE;
  hans.target_pos_B = (INT16U) target.positionB * TICKS_PER_DEGREE;
  hans.pwm_motor_A = (INT16S) pwm.motorA;
  hans.pwm_motor_B = (INT16S) pwm.motorB;

  xQueueSend(log_status_queue,&hans,portMAX_DELAY);
}
