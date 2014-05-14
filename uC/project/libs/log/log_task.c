/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: log_task.c
 *
 * PROJECT....: updates the log
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140512  NI   Module created.
 *
 *****************************************************************************/
/***************************** Include files *******************************/

#include "log_task.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"

/******************************** Variables *********************************/
log_file_type log_global[MAX_LOG_ENTRIES];
static log_file_type log_has_been_printed = {1, 2, 3, 4, 5, 6}; 
static log_file_type log_has_been_reset   = {0, 0, 0, 0, 0, 0};
static log_file_type log_can_be_printed   = {6, 5, 4, 3, 2, 1};

/*****************************   Functions   *******************************/

void log_task(void *pvParameters)
{
  INT8U index = 1;

  if( xSemaphoreTake(interface_log_sem, portMAX_DELAY) )
  {
    reset_log(log_global);
    xSemaphoreGive(interface_log_sem);
  }
  PRINTF("log task is started\n");
  while(1)
  {    
    if( xSemaphoreTake(interface_log_sem, portMAX_DELAY) )
    {
      if( log_global[0].current_pos_A == 0 &&
          log_global[0].current_pos_B == 0 &&
          log_global[0].target_pos_A == 0 &&
          log_global[0].target_pos_B == 0 &&
          log_global[0].pwm_motor_A == 0 &&
          log_global[0].pwm_motor_B == 0 )
      {
        while( uxQueueMessagesWaiting(log_status_queue) > 0 )
        {
          if(index >= ( MAX_LOG_ENTRIES ) )
          { 
            log_global[0] = log_can_be_printed;
            break;
          }
          else if( xQueueReceive(log_status_queue, &log_global[index], portMAX_DELAY) )
          {
            //PRINTF("GOT MESSAGE\n");
            index++;
          }
        }
      }
      else if(  log_global[0].current_pos_A == log_has_been_printed.current_pos_A &&
          log_global[0].current_pos_B == log_has_been_printed.current_pos_B &&
          log_global[0].target_pos_A  == log_has_been_printed.target_pos_A  &&
          log_global[0].target_pos_B  == log_has_been_printed.target_pos_B  &&
          log_global[0].pwm_motor_A   == log_has_been_printed.pwm_motor_A   &&
          log_global[0].pwm_motor_B   == log_has_been_printed.pwm_motor_B )
      {
        index = 1;
        reset_log(log_global);
      }
      xSemaphoreGive(interface_log_sem);
    }
    _wait(LOG_TASK_CYCLE);
  }
}

void print_log(log_file_type log[MAX_LOG_ENTRIES])
{
  INT8U x;

  if( ( log_global[0].current_pos_A == log_can_be_printed.current_pos_A &&
      log_global[0].current_pos_B == log_can_be_printed.current_pos_B &&
      log_global[0].target_pos_A  == log_can_be_printed.target_pos_A  &&
      log_global[0].target_pos_B  == log_can_be_printed.target_pos_B  &&
      log_global[0].pwm_motor_A   == log_can_be_printed.pwm_motor_A   &&
      log_global[0].pwm_motor_B   == log_can_be_printed.pwm_motor_B ) )
  {
    for(x = 1; x < MAX_LOG_ENTRIES; x++)
    {
      PRINTF(
          "%u, \t\t%u, \t\t%u, \t\t%u, \t\t%d, \t\t%d\n",
          log[x].current_pos_A,
          log[x].current_pos_B,
          log[x].target_pos_A,
          log[x].target_pos_B,
          log[x].pwm_motor_A,
          log[x].pwm_motor_B
      );
    }
    log[0] = log_has_been_printed; 
  }
}

void display_log_format(void)
{
  PRINTF(
      "Position A:\t"
      "Position B:\t"
      "Target A:\t"
      "Target B:\t"
      "PWM A:\t\t"
      "PWM B:\n"
  );
  //kill_all(unicorns, happiness);
}
void reset_log(log_file_type log[MAX_LOG_ENTRIES] )
{
  INT8U x;

  for(x = 0; x < MAX_LOG_ENTRIES; x++)
  {
    log[x].current_pos_A = 0;
    log[x].current_pos_B = 0;
    log[x].target_pos_A = 0;
    log[x].target_pos_B = 0;
    log[x].pwm_motor_A = 0;
    log[x].pwm_motor_B = 0;
  }
}
