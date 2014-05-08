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

#include "read_pos.h"
#include "queue/queue_ini.h"

/*****************************   Functions   *******************************/
void read_pos_task(void *pvParameters)
{
  INT8U index = 0;
  coordinate_type coordinate;
  coordinate_type list[LIST_SIZE];
  
  init_list(list);
  while(1)
  {
    if( uxQueueMessagesWaiting(pos_ctrl_queue) > 0 ) // if someone gives an coordinate it takes priority
    {
      xQueueReceive(pos_ctrl_queue, &coordinate , 0 );
      while( !xQueueSend(target_pos_queue, &coordinate , MILISEC(1000) ) );
    }
    else
    {
      coordinate = list[index];
      if ( xQueueSend(target_pos_queue, &coordinate, MILISEC(15000) ) )
      {
        if( index++ == LIST_SIZE )
        {
          index = 0; //to prevent overflow
        }
        vTaskDelay(MILLI_SEC(16)); //We should actually wait 16 2/3 mili seconds if we want 60 frames per second... Maybe we should do this another way
      }
      else
      {
        index = 0; //if nothing is received in 15 seconds.
      }
    }
  }
}

void init_list( coordinate_type final_list[LIST_SIZE] )
{
  INT16U i;
  INT8U j = 0;
  
  FP32 matlab_liste[MATLAB_LIST_SIZE] = {
  //paste matlab generated list here... I can handle FP32
  //if we find that we need another data type we should correct this in the struct definition as well..
  12.3,03.6,10.0
  };
  
  for( i = 0; i < MATLAB_LIST_SIZE; i++)
  {
    if( ( i % 3 ) == 0 ) final_list[j].x = matlab_liste[i];
    if( ( i % 3 ) == 1 ) final_list[j].x = matlab_liste[i];
    if( ( i % 3 ) == 2 ) final_list[j++].x = matlab_liste[i];
  }
}