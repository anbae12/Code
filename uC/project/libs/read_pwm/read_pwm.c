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
#define END_PWM            (40*PWM_PERCENT) //hvis i vil have at den går til 40% ved slut af test.

/***************************** Include files *******************************/

#include "configs/project_settings.h"
#include "read_pwm.h"
#include "queue/queue_ini.h"
#include "matlab_pwm_liste.h"

/******************************** Variables *********************************/
pwm_duty_cycle_type interface_pwm;
pwm_duty_cycle_type target_pwm;

/*****************************   Functions   *******************************/
void read_pwm_task(void *pvParameters)
{
  PRINTF("read_pwm has started\n");

  INT16U index = 0;
  pwm_duty_cycle_type pwm = {.motorA = 0, .motorB = 0};

  /*for timing*/
  portTickType last_wake_time;
  last_wake_time = xTaskGetTickCount();  
  while(1)
  {
    if( index < PWM_LIST_SIZE )
    {
      pwm = list[index++];
    }
    else
    {
      pwm.motorA = END_PWM;
      pwm.motorB = 0;
    }

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
    
   vTaskDelayUntil(&last_wake_time, READ_PWM_TASK_CYCLE);
  }
}


void init_pwm_list( pwm_duty_cycle_type final_list[PWM_LIST_SIZE] )
{
  // INT16U i;
  // INT8U j = 0;
  // if(0)
  // {
  // INT8U matlab_liste[MATLAB_PWM_LIST_SIZE] = {
  // //paste matlab generated list here... I can handle FP32
  // // if we find that we need another data type we should correct this in the struct definition as well..
     // 0
  // };
  // for( i = 0; i < MATLAB_PWM_LIST_SIZE; i++) 
  // {
    // if( ( i % 2 ) == 0 ) final_list[j].motorA = (INT16S) matlab_liste[i] * 2048/100;
    // if( ( i % 2 ) == 1 ) final_list[j++].motorB = (INT16S) matlab_liste[i] * 2048/100;
  // }
  // }
}