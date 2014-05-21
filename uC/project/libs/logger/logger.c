/*****************************************************************************
 *
 * MODULENAME: logger
 * DESCRIPTION:
 ****************************************************************************/
/****************************** Include files *******************************/
#include "configs/project_settings.h"
#include "inc/emp_type.h"
#include "logger.h"
#include "SPI/spi.h"
/********************************* Defines **********************************/
#define         LOG_SIZE_MAX                    395
#define         LOG_INPUT_QUEUE_SIZE            445

/******************************** Constants *********************************/

/******************************** Variables *********************************/

xSemaphoreHandle log_mutex;
INT16U no_of_entries; //index.
static log_entry log[LOG_SIZE_MAX] = {{0,0}}; //the log.
xQueueHandle log_input_queue;        //the log entries input queue (ctrl_task -> logger_task).

/******************************** Functions *********************************/

void init_logger_presched()
{
  no_of_entries=0;
  log_mutex=xSemaphoreCreateMutex();
  log_input_queue=xQueueCreate(LOG_INPUT_QUEUE_SIZE,sizeof(log_entry));
}

void logger_task(void *pvParameters)
{
  log_entry local_entry;
  while(1)
  {
    //Blocks if no entries have been gived to the input queue.
    while(1)
    {
      _QUEUE_TAKE_BLOCKING(log_input_queue,&local_entry);

      _MTX_TAKE_BLOCKING(log_mutex);
      if(no_of_entries>LOG_SIZE_MAX)
      {
        //@TODO: Handle the log overflow, signal error somehow!
      }
      else
      {
        log[no_of_entries++]=local_entry;
      }
      _MTX_GIVE(log_mutex);

    }
  }
}

void log_flush(BOOLEAN print)
{
  INT32S i;
  INT16U n;
  _MTX_TAKE_BLOCKING(log_mutex);
  n=no_of_entries;
  no_of_entries=0;
  if(print)
  {
    for(i=0;i<n;i++)
    {
      PRINTF(
          "%u, \t\t%u, \t\t%u, \t\t%u, \t\t%d, \t\t%d\n",
          log[i].current_posA,
          log[i].current_posB,
          log[i].setpointA,
          log[i].setpointB,
          log[i].pwmA,
          log[i].pwmB
      );
    }
  }
  _MTX_GIVE(log_mutex);
}

void log_entry_register(pwm_duty_cycle_type pwm, motor_pos current, motor_pos target)
{
  log_entry temp_entry;
  temp_entry.current_posA = (INT16U) current.positionA;
  temp_entry.current_posB = (INT16U) current.positionB;
  temp_entry.setpointA = (INT16U) target.positionA;
  temp_entry.setpointB = (INT16U) target.positionB;
  temp_entry.pwmA = (INT16S) pwm.motorA;
  temp_entry.pwmB = (INT16S) pwm.motorB;
  if( !xQueueSend(log_input_queue,&temp_entry,0))
    PRINTF("queue is full\n");

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
}

/****************************** END OF MODULE *******************************/
