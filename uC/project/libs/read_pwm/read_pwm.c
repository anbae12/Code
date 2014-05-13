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

#include "configs/project_settings.h"
#include "read_pwm.h"
#include "queue/queue_ini.h"

/******************************** Variables *********************************/
pwm_duty_cycle_type interface_pwm;
pwm_duty_cycle_type target_pwm;

/*****************************   Functions   *******************************/
void read_pwm_task(void *pvParameters)
{

  PRINTF("read_pwm has started\n");

  INT8U index = 0;
  pwm_duty_cycle_type pwm = {.motorA = 0, .motorB = 0};
  pwm_duty_cycle_type list[PWM_LIST_SIZE];
  
  init_pwm_list(list);

  // for timing
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
      pwm.motorA = 0;
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

   vTaskDelayUntil(&last_wake_time, MILLI_SEC(READ_PWM_TASK_CYCLE));
  }
}


void init_pwm_list( pwm_duty_cycle_type final_list[PWM_LIST_SIZE] )
{
  INT16U i;
  INT8U j = 0;
  
  INT8U matlab_liste[MATLAB_PWM_LIST_SIZE] = {
      0,0,
      1,0,
      2,0,
      3,0,
      4,0,
      5,0,
      6,0,
      7,0,
      8,0,
      9,0,
      10,0,
      11,0,
      12,0,
      13,0,
      14,0,
      15,0,
      16,0,
      17,0,
      18,0,
      19,0,
      20,0,
      21,0,
      22,0,
      23,0,
      24,0,
      25,0,
      26,0,
      27,0,
      28,0,
      29,0,
      30,0,
      31,0,
      32,0,
      33,0,
      34,0,
      35,0,
      36,0,
      37,0,
      38,0,
      39,0,
      40,0,
      41,0,
      42,0,
      43,0,
      44,0,
      45,0,
      46,0,
      47,0,
      48,0,
      49,0,
      50,0,
      51,0,
      52,0,
      53,0,
      54,0,
      55,0,
      56,0,
      57,0,
      58,0,
      59,0,
      60,0,
      61,0,
      62,0,
      63,0,
      64,0,
      65,0,
      66,0,
      67,0,
      68,0,
      69,0,
      70,0,
      71,0,
      72,0,
      73,0,
      74,0,
      75,0,
      76,0,
      77,0,
      78,0,
      79,0,
      80,0,
      81,0,
      82,0,
      83,0,
      84,0,
      85,0,
      86,0,
      87,0,
      88,0,
      89,0,
      90,0,
      91,0,
      92,0,
      93,0,
      94,0,
      95,0,
      96,0,
      97,0,
      98,0,
      99,0,
      100,0
  };
  
  for( i = 0; i < MATLAB_PWM_LIST_SIZE; i++) 
  {
    if( ( i % 2 ) == 0 ) final_list[j].motorA = (INT16S) matlab_liste[i];
    if( ( i % 2 ) == 1 ) final_list[j++].motorB = (INT16S) matlab_liste[i];
  }
}
