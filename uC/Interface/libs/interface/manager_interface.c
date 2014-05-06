/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME: manager_interface
 * DESCRIPTION:
 ****************************************************************************/
/****************************** Include files *******************************/
#include "string.h"
#include "inc/lm3s6965.h"
#include "inc/glob_def.h"
#include "manager_interface.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "configs/project_settings.h"
#include "uart/uart.h"
#include "uart/uartprintf.h"
// #include "log/log_task.h"
// #include "text.h"


/********************************* Defines **********************************/

//Remember that strcmp() returns 0 when strings are equal.
#define UI_CMD_RESET "reset"  //find reset position
#define UI_CMD_START "start"  //Start tracking target
#define UI_CMD_STOP "stop"    //emergency stop
#define UI_CMD_COORDINATE "C xxx.yyy.zzz"//Gå til koordinatsæt 
#define UI_CMD_READ "read" //Læs sensorværdier. 

//States:
#define ST_UI_CMD               0
#define ST_UI_COORDINATE        1

/******************************** Constants *********************************/

/******************************** Variables *********************************/

/******************************** Functions *********************************/

void interface_task(void *pvParameters)
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  char mirror_string[UART_QUEUE_LEN] = {0};

  char tempstring[UART_QUEUE_LEN] = {0};

  INT8U state=ST_UI_CMD;
  
  INT8U coord[COORDINATE_LEN] = {0};
  
  UARTprintf("Program started.\n\r");
  interface_display_commands();

  while(1)
  {
    if(uart_pop_string_echo(mirror_string,UART_QUEUE_LEN,TRUE))
    {
      switch(state)
      {
      case ST_UI_CMD:
        if(!strcmp(UI_CMD_START,mirror_string))
		{
			//set pos_ctrl
			//set ctrl_bit
          UARTprintf("set pos ctrl");
		}
        else if(!strcmp(UI_CMD_STOP,mirror_string))
		{
			//set ctrl_bit
          UARTprintf("set ctrl bit");
		}
        else if (!strcmp(UI_CMD_READ,mirror_string))
		{
			//print log
          UARTprintf("print log");
		}
        else if(!strcmp(UI_CMD_RESET,mirror_string))
		{
			INT8U index;
			for(index = 0; index < COORDINATE_LEN; index++)
			{
				coord[index] = 0;
			}
			state = ST_UI_COORDINATE;
		}
        
		else if(mirror_string[0] == 'C' && mirror_string[5] == '.' && mirror_string[9] == '.')
        { 
			INT8U index;
			for(index = 0; index < COORDINATE_LEN; index++)
			{
				coord[index] = mirror_string[index + 2];
			}
			state = ST_UI_COORDINATE;
        }
        else
		{
          UARTprintf("Invalid command entered.\n\r");
          interface_display_commands();
		}
        break;
      case ST_UI_COORDINATE:
        //if( set_coord )
		// you should have it in coord now
        if( TRUE )
        {
          state=ST_UI_CMD;
        }
        break;
      }
    }

    _wait(MILLI_SEC(INTERFACE_TASK_DELAY_MS));
  }
}

void interface_display_commands()
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  UARTprintf(
		"Enter <%s> to get a readable list\n\r"
		"Enter <%s> to start following the predefined parable\n\r"
		"Enter <%s> for emergency stop\n\r"
		"Enter <%s> to move to reset position\n\r"
		"Enter <%s> to go to a specific coordinate\n\r",
		UI_CMD_READ,
		UI_CMD_START,
		UI_CMD_STOP,
		UI_CMD_RESET,
		UI_CMD_COORDINATE
		);
}


/****************************** END OF MODULE *******************************/
