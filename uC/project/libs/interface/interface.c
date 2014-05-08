/*****************************************************************************
 * Mikkel, �se & Mikael
 * MODULENAME: manager_interface
 * DESCRIPTION:
 ****************************************************************************/
/****************************** Include files *******************************/
#include "string.h"
#include "inc/lm3s6965.h"
#include "inc/glob_def.h"
#include "interface.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "configs/project_settings.h"
#include "uart/uart.h"
#include "uart/uartprintf.h"
#include "read_pos/read_pos.h"
#include "queue/queue_ini.h"

//#include "debug/ftoa.h"


/********************************* Defines **********************************/

//Remember that strcmp() returns 0 when strings are equal.
#define UI_CMD_RESET "reset"  //find reset position
#define UI_CMD_START "start"  //Start tracking target
#define UI_CMD_STOP "stop"    //emergency stop
#define UI_CMD_COORDINATE "C xxx.yyy.zzz"//G� til koordinats�t 
#define UI_CMD_READ "read" //L�s sensorv�rdier. 
#define UI_CMD_OPEN_LOOP "open"
#define COORDINATE_LEN 11



/******************************** Constants *********************************/

/******************************** Variables *********************************/


/******************************** Functions *********************************/

coordinate_type input_coordinate( INT8U coord[11])
{
  coordinate_type coordinate;
  INT16U temp;

  temp = (coord[0] *100 )+ (coord[1] *10) + coord[2];
  coordinate.x = (FP32) temp;
  temp = (coord[4] *100 )+ (coord[5] *10) + coord[6];
  coordinate.y = (FP32) temp;
  temp = (coord[8] *100 )+ (coord[9] *10) + coord[10];
  coordinate.z = (FP32) temp;

  return coordinate;
}


void interface_task(void *pvParameters)
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  char mirror_string[UART_QUEUE_LEN] = {0};

  char tempstring[UART_QUEUE_LEN] = {0};

  INT8U coord[COORDINATE_LEN] = {0};

  UARTprintf("Program started.\n");
  interface_display_commands();

  while(1)
  {
    if(uart_pop_string_echo(mirror_string,UART_QUEUE_LEN,TRUE))
    {
      if(!strcmp(UI_CMD_START,mirror_string))
      {

        if( xSemaphoreTake(position_ctrl_sem, portMAX_DELAY) )
        {
          interface_coordinate = invalid_coordinate;
          xSemaphoreGive(position_ctrl_sem);
        }



        UARTprintf("set pos ctrl\n");
      }
      else if(!strcmp(UI_CMD_STOP,mirror_string))
      {

        //set ctrl_bit
        UARTprintf("set ctrl bit\n");
      }
      else if (!strcmp(UI_CMD_READ,mirror_string))
      {
        //print log
        UARTprintf("print log\n");
      }
      else if (!strcmp(UI_CMD_OPEN_LOOP,mirror_string))
      {
        //test open loop
        UARTprintf("Ramp speed up\n");
        UARTprintf("Ramp speed down\n");
      }
      else if(!strcmp(UI_CMD_RESET,mirror_string))
      {
        INT8U index;
        for(index = 0; index < COORDINATE_LEN; index++)
        {
          coord[index] = 0;
        }
        if( xSemaphoreTake(position_ctrl_sem, portMAX_DELAY) )
        {
          interface_coordinate = input_coordinate(coord);
          xSemaphoreGive(position_ctrl_sem);
        }
      }

      else if(mirror_string[0] == 'C' && mirror_string[5] == '.' && mirror_string[9] == '.')
      {
        INT8U index;
        for(index = 0; index < COORDINATE_LEN; index++)
        {
          coord[index] = mirror_string[index + 2] - '0';
        }
        if( xSemaphoreTake(position_ctrl_sem, portMAX_DELAY) )
        {
          interface_coordinate = input_coordinate(coord);
          xSemaphoreGive(position_ctrl_sem);
        }
      }
      else
      {
        UARTprintf("Invalid command entered.\n");
        interface_display_commands();
      }
    }
    _wait(MILLI_SEC(10));
  }
}

void interface_display_commands()
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  UARTprintf(
      "Enter <%s> to get a readable list\n"
      "Enter <%s> to start following the predefined parabola\n"
      "Enter <%s> for emergency stop\n"
      "Enter <%s> to test open loop\n"
      "Enter <%s> to move to reset position\n"
      "Enter <%s> to go to a specific coordinate\n",
      UI_CMD_READ,
      UI_CMD_START,
      UI_CMD_STOP,
      UI_CMD_OPEN_LOOP,
      UI_CMD_RESET,
      UI_CMD_COORDINATE
  );
}


/****************************** END OF MODULE *******************************/
