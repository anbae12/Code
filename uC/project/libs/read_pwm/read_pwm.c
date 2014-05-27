/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: read_pwm.c
 *
 * PROJECT....: spits out a new pwm periodically.
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
#define READ_PWM_TASK_CYCLE 1
#include "configs/project_settings.h"
#include "read_pwm.h"
#include "queue/queue_ini.h"
#include "matlab_pwm_liste.h"

/******************************** Variables *********************************/
pwm_duty_cycle_type interface_pwm;
pwm_duty_cycle_type target_pwm;
/*****************************   Functions   *******************************/

pwm_duty_cycle_type read_pwm_function( INT8U reset )
{
  static INT16U index = 0;
  static pwm_duty_cycle_type pwm;
  
  if(reset)
  {
    index = 0;
  }

  if( index < PWM_LIST_SIZE )
  {
    pwm = list[index++];
  }
  else
  {
    pwm.motorA = END_PWM;
    pwm.motorB = 0;
  }

  return pwm;
}
