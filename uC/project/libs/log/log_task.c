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
#define LOG_TASK_CYCLE 9001
/***************************** Include files *******************************/

#include "log_task.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"

/******************************** Variables *********************************/
log_file_type log_global[MAX_LOG_ENTRIES];
static log_file_type log_has_been_printed = {1, 2, 3, 4, 5, 6}; 
static log_file_type log_has_been_reset   = {0, 0, 0, 0, 0, 0};

/*****************************   Functions   *******************************/

void log_task(void *pvParameters)
{
  INT8U index = 0;

  if( xSemaphoreTake(interface_log_sem, portMAX_DELAY) )
  {
    reset_log(log_global);
    xSemaphoreGive(interface_log_sem);
  }
  while(1)
  {    
    if( xSemaphoreTake(interface_log_sem, portMAX_DELAY) )
    {
      if( log_global[MAX_LOG_ENTRIES - 1].current_pos_A == 0 &&
          log_global[MAX_LOG_ENTRIES - 1].current_pos_B == 0 &&
          log_global[MAX_LOG_ENTRIES - 1].target_pos_A == 0 &&
          log_global[MAX_LOG_ENTRIES - 1].target_pos_B == 0 &&
          log_global[MAX_LOG_ENTRIES - 1].pwm_motor_A == 0 &&
          log_global[MAX_LOG_ENTRIES - 1].pwm_motor_B == 0 )
      {
        while( uxQueueMessagesWaiting(log_status_queue) > 0 )
        {
          if(index >= ( MAX_LOG_ENTRIES - 1 ) )
          { 
            PRINTF("FULL BUFFER\n");
            break;
          }
          else if( xQueueReceive(log_status_queue, &log_global[index], portMAX_DELAY) )
          {
            index++;
          }
        }
      }
      else if(  log_global[MAX_LOG_ENTRIES - 1].current_pos_A == log_has_been_printed.current_pos_A &&
                log_global[MAX_LOG_ENTRIES - 1].current_pos_B == log_has_been_printed.current_pos_B &&
                log_global[MAX_LOG_ENTRIES - 1].target_pos_A  == log_has_been_printed.target_pos_A  &&
                log_global[MAX_LOG_ENTRIES - 1].target_pos_B  == log_has_been_printed.target_pos_B  &&
                log_global[MAX_LOG_ENTRIES - 1].pwm_motor_A   == log_has_been_printed.pwm_motor_A   &&
                log_global[MAX_LOG_ENTRIES - 1].pwm_motor_B   == log_has_been_printed.pwm_motor_B )
      {
        index = 0;
        reset_log(log_global);
      }
      xSemaphoreGive(interface_log_sem);
    }
    _wait(MILLI_SEC(LOG_TASK_CYCLE));
  }
}

void print_log(log_file_type log[MAX_LOG_ENTRIES])
{
  INT8U x;

  PRINTF(
  "Target A:\t"
  "Target B:\t"
  "Position A:\t"
  "Position B:\t"
  "PWM A:\t"
  "PWM B:\n"
  );
  
  for(x = 0; x < MAX_LOG_ENTRIES - 1; x++)
  {
    PRINTF(
    ", %u, %u, %u, %u, %u, %u",
    log[x].current_pos_A,
    log[x].current_pos_B,
    log[x].target_pos_A,
    log[x].target_pos_B,
    log[x].pwm_motor_A,
    log[x].pwm_motor_B
    );
  }
  log[MAX_LOG_ENTRIES - 1] = log_has_been_printed; 
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
