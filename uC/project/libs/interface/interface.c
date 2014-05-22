/*****************************************************************************
 * MODULENAME: interface
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
#include "read_pos/read_pos_upsampling.h"
#include "read_pwm/read_pwm.h"
#include "ctrl/ctrl_task.h"
#include "queue/queue_ini.h"
#include "logger/logger.h"

//#include "debug/ftoa.h"


/********************************* Defines **********************************/

//Remember that strcmp() returns 0 when strings are equal.
#define UI_CMD_RESET "reset"  //find reset position
#define UI_CMD_START "start"  //Start tracking target
#define UI_CMD_STOP "stop"    //emergency stop
#define UI_CMD_COORDINATE "C +xx.+yy.+zz"//Gå til koordinatsæt 
#define UI_CMD_PWM "PA+99"//Set pwm
#define UI_CMD_OPEN_LOOP "open"
#define UI_CMD_ANGLE "A aaaa.bbbb" 
#define COORDINATE_LEN 11



/******************************** Constants *********************************/

/******************************** Variables *********************************/


/******************************** Functions *********************************/
motor_pos input_position( INT8U coord[11])
{
  // /* this assumes the format aaaa.bbbb */
  motor_pos pos;

  pos.positionA = (FP32) ( (coord[0] *1000 )+ (coord[1] *100) + (coord[2] *10) + coord[3] );
  pos.positionB = (FP32) ( (coord[5] *1000 )+ (coord[6] *100) + (coord[7] *10) + coord[8] );

  if(pos.positionA > 1079)
  {
    pos.positionA = 0;
  }
  if( (pos.positionB > 550) && (pos.positionB < 900) )
  {
    pos.positionB = 900;
  }
  if( (pos.positionB > 200 ) && (pos.positionB <= 550 ) )
  {
    pos.positionB = 200;
  }
  if( (pos.positionB > 1079) )
  {
    pos.positionB = 0;
  }
  return pos;
}

coordinate_type input_coordinate( INT8U coord[11])
{
  /* this assumes the format +xx.+yy.+zz */
  coordinate_type coordinate;
  INT16S temp;
  
  temp = (coord[1] *10) + coord[2];
  temp = (coord[4] *100 )+ (coord[5] *10) + coord[6];
  temp = (coord[8] *100 )+ (coord[9] *10) + coord[10];
  if(coord[0] == '-')
  {
    temp *= -1;
  }
  if(coord[4] == '-')
  {
    temp *= -1;
  }
  if(coord[8] == '-')
  {
    temp *= -1;
  }
  coordinate.x = (FP32) temp;
  coordinate.y = (FP32) temp;
  coordinate.z = (FP32) temp;

  return coordinate;
}


void interface_task(void *pvParameters)
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  char mirror_string[UART_QUEUE_LEN] = {0};

  INT8U coord[COORDINATE_LEN] = {0};
  INT8U next_control_state;
  PRINTF("\n\n\nProgram started.\n");
  interface_display_commands();

  static INT8U dummy_ptr = 1;
  static motor_pos input_angle_pos;
  static coordinate_type input_cart_pos;
  static pwm_duty_cycle_type input_pwm;
  
  while(1)
  {
    if(uart_pop_string_echo(mirror_string,UART_QUEUE_LEN,TRUE))
    {
      if(!strcmp(UI_CMD_START,mirror_string))
      {
        next_control_state = SKEET_SHOOT_DEMO; 
        control_set_state(next_control_state, &dummy_ptr);
        PRINTF("set pos ctrl\n");
      }
      else if(!strcmp(UI_CMD_STOP,mirror_string))
      {
        next_control_state = STOP_MOTORS_NOW;
        control_set_state(next_control_state, &dummy_ptr);
      }
      else if (!strcmp(UI_CMD_OPEN_LOOP,mirror_string))
      {
        next_control_state = OPEN_LOOP_TEST; 
        control_set_state(next_control_state, &dummy_ptr);
      }
      else if(!strcmp(UI_CMD_RESET,mirror_string))
      {
        next_control_state = SINGLE_ANGLE_POSITION; 
        input_angle_pos.positionA = RESET_POS_TILT; 
        input_angle_pos.positionB = RESET_POS_PAN;
        control_set_state(next_control_state, &input_angle_pos);
      }
      else if(mirror_string[0] == 'C' && mirror_string[5] == '.' && mirror_string[9] == '.' && strlen(mirror_string) == 13)
      {
        next_control_state = SINGLE_CARTESIAN_POSITION; 
        INT8U index;
        for(index = 0; index < COORDINATE_LEN; index++)
        {
          coord[index] = mirror_string[index + 2] - '0';
        }
        input_cart_pos = input_coordinate(coord);
        control_set_state(next_control_state, &input_cart_pos);
      }
      else if(mirror_string[0] == 'P' && strlen(mirror_string) == 5)
      {
        next_control_state = SINGLE_PWM_MODE;
        INT16S input_converted;
        input_converted = (mirror_string[3] - '0') * 10;
        input_converted += mirror_string[4] - '0';

        if(mirror_string[2] == '-')
        {
          input_converted *= -1;
        }
        input_converted *= PWM_PERCENT;
        if(mirror_string[1] == 'A')
        {
          input_pwm.motorA = input_converted;
        }
        else if(mirror_string[1] == 'B')
        {
          input_pwm.motorB = input_converted;
        }
        control_set_state(next_control_state, &input_pwm); 
      }
      else if(mirror_string[0] == 'A' && mirror_string[6] == '.' && strlen(mirror_string) == 11)
      {
        static motor_pos interface_position;
        next_control_state = SINGLE_ANGLE_POSITION;
        INT8U index;
        for(index = 0; index < 9; index++)
        {
          coord[index] = mirror_string[index + 2] - '0';
        }
        interface_position = input_position(coord);
        
        control_set_state(next_control_state, &interface_position);
      }
      else
      {
        PRINTF("Invalid command entered.\n");
        interface_display_commands();
      }
    }
    else
    {
      log_flush(TRUE);
    }
  }
  _wait(INTERFACE_TASK_CYCLE);
}

void interface_display_commands()
/*****************************************************************************
 * Function: See header file
 ****************************************************************************/
{
  PRINTF(
      "Enter <%s> to start following the predefined parabola\n"
      "Enter <%s> for emergency stop\n"
      "Enter <%s> to test open loop\n"
      "Enter <%s> to move to reset position\n"
      "Enter <%s> to go to a Cartesian coordinate\n"
      "Enter <%s> to go to a Spherical coordinate\n"
      "Enter <%s> to force a PWM on a single motor\n",
      UI_CMD_START,
      UI_CMD_STOP,
      UI_CMD_OPEN_LOOP,
      UI_CMD_RESET,
      UI_CMD_COORDINATE,
      UI_CMD_ANGLE,
      UI_CMD_PWM
  );
}


/****************************** END OF MODULE *******************************/
