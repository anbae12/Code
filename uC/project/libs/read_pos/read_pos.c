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
#define READ_POS_TASK_FREQUENCY 6000

/***************************** Include files *******************************/

#include "read_pos.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"

/******************************** Variables *********************************/
coordinate_type interface_coordinate;
coordinate_type target_var;

/*****************************   Functions   *******************************/

void read_pos_debug(coordinate_type coordinate)//strictly for debugging purposes... This is why it is not mentioned anywhere else...
{
  if( READ_POS_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) coordinate.x;
    PRINTF("Coordinate x: %d\t",conv);
    conv = (INT16U) coordinate.y;
    PRINTF("Coordinate y: %d\t",conv);
    conv = (INT16U) coordinate.z;
    PRINTF("Coordinate z: %d\n",conv);
  }
}

void read_pos_task(void *pvParameters)
{
  INT8U index = 0;
  coordinate_type coordinate = {.y = 12, .x = 111, .z = 222};
  coordinate_type list[LIST_SIZE];
  
  init_list(list);

  //for timing
  portTickType last_wake_time;

  last_wake_time = xTaskGetTickCount();

  while(1)
  {
    if( index < LIST_SIZE - 2 )
    {
      coordinate = list[index++];
    }
    else
    {
      coordinate.x = 0;
      coordinate.y = 0;
      coordinate.z = 0;
    }

    if( xSemaphoreTake(position_ctrl_sem, portMAX_DELAY) )
    {
      if( interface_coordinate.x != invalid_coordinate.x &&
          interface_coordinate.y != invalid_coordinate.y &&
          interface_coordinate.z != invalid_coordinate.z )
      {
        coordinate = interface_coordinate;
        index = 0;
      }
      xSemaphoreGive(position_ctrl_sem);
    }

    if( xSemaphoreTake(target_var_sem, portMAX_DELAY) )
    {
      target_var = coordinate;
      PRINTF("I AM ALIVE");
      xSemaphoreGive(target_var_sem);
    }

    read_pos_debug(coordinate);

    vTaskDelayUntil(&last_wake_time, MILLI_SEC(READ_POS_TASK_FREQUENCY));

  }
}


void init_list( coordinate_type final_list[LIST_SIZE] )
{
  INT16U i;
  INT8U j = 0;
  
  FP32 matlab_liste[MATLAB_LIST_SIZE] = {
  //paste matlab generated list here... I can handle FP32
  //if we find that we need another data type we should correct this in the struct definition as well..
      12.3,14,15,
      12.3,03.6,10.0,
      12.3,04.6,10.0,
      12.3,05.6,10.0,
      12.3,06.6,10.0
  };
  
  for( i = 0; i < MATLAB_LIST_SIZE; i++) //TODO this list is not correct...
  {
    if( ( i % 3 ) == 0 ) final_list[j].x = matlab_liste[i];
    if( ( i % 3 ) == 1 ) final_list[j].y = matlab_liste[i];
    if( ( i % 3 ) == 2 ) final_list[j++].z = matlab_liste[i];
  }
}
