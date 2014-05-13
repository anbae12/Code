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
#define READ_PWM_TASK_CYCLE 1000

/***************************** Include files *******************************/

#include "read_pwm.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"

/******************************** Variables *********************************/
pwm_duty_cycle_type interface_pwm;
pwm_duty_cycle_type target_pwm;

/*****************************   Functions   *******************************/

void read_pwm_task(void *pvParameters)
{
  _wait(3);
  PRINTF("read_pwm has started\n");

  INT8U index = 0;
  pwm_duty_cycle_type pwm = {.motorA = 100, .motorB = 100};
  pwm_duty_cycle_type list[PWM_LIST_SIZE];
  
  init_pwm_list(list);

  //for timing
  portTickType last_wake_time;

//  last_wake_time = xTaskGetTickCount();
  while(1)
  {
    if( index < (PWM_LIST_SIZE - 1) )
    {
      pwm = list[index++];
    }
    else
    {
      pwm.motorA = 0;
      pwm.motorB = 0;
    }
    PRINTF("list: %d",list[index].motorA);

    if( xSemaphoreTake(interface_pwm_sem, portMAX_DELAY) )
    {
      if( interface_pwm.motorA != invalid_pwm.motorA &&
          interface_pwm.motorB != invalid_pwm.motorB  )
      {
        pwm = interface_pwm;
        index = 0;
      }
      xSemaphoreGive(interface_pwm_sem);
    }

    if( xSemaphoreTake(target_pwm_sem, portMAX_DELAY) )
    {
      target_pwm = pwm;
      xSemaphoreGive(target_pwm_sem);
    }

    _wait(MILLI_SEC(READ_PWM_TASK_CYCLE));
//    vTaskDelayUntil(&last_wake_time, MILLI_SEC(READ_PWM_TASK_CYCLE));
  }
}


void init_pwm_list( pwm_duty_cycle_type final_list[PWM_LIST_SIZE] )
{
  INT16U i;
  INT8U j = 0;
  
  INT8U matlab_liste[MATLAB_PWM_LIST_SIZE] = {
  //paste matlab generated list here... I can handle FP32
  //if we find that we need another data type we should correct this in the struct definition as well..
      0,0,
      1,0,
      2,0,
      3,0,
      4,0,
      5,0
  };
  
  for( i = 0; i < MATLAB_PWM_LIST_SIZE; i++) 
  {
    if( ( i % 2 ) == 0 ) final_list[j].motorA = matlab_liste[i];
    if( ( i % 2 ) == 1 ) final_list[j++].motorB = matlab_liste[i];
  }
}
