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
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "inc/glob_def.h"
#include "configs/project_settings.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "configs/project_settings.h"
#include "SPI/spi.h"
#include "queue/queue_ini.h"
#include "read_pwm/read_pwm.h"
#include "read_pos/read_pos_upsampling.h"
#include "ctrl_task.h"
#include <math.h>
#include "controller.h"
#include "logger/logger.h"
#include "read_pwm/matlab_pwm_liste.h"
/*****************************    Defines    *******************************/
#define PI 3.14159265359
#define TICKS_PER_DEGREE 1080/360 //I know this is 3 but this is more descriptive

/******************************** Variables *********************************/
INT8U interface_to_control_byte = 0b10000000;

void current_pos_debug(motor_pos target)
{
  if ( CTRL_CURRENT_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) target.positionA * TICKS_PER_DEGREE;
    PRINTF("current ticks: %d\t",conv);
    conv = (INT16U) target.positionB * TICKS_PER_DEGREE;
    PRINTF("current ticks: %d\n",conv);
  }
}

void target_pwm_debug(pwm_duty_cycle_type target)
{
  if ( TARGET_PWM_DEBUG )
  {
    INT16S conv;
    conv = (INT16S) target.motorA;
    PRINTF("target pwm A: %d\t",conv);
    conv = (INT16S) target.motorB;
    PRINTF("target pwm B: %d\n",conv);
  }
}

void target_pos_debug(motor_pos target)
{
  if ( CTRL_TARGET_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) target.positionA;
    PRINTF("target phi: %d\t",conv);
    conv = (INT16U) target.positionB;
    PRINTF("target theta: %d\n",conv);
  }
}

void pwm_spi_debug(pwm_duty_cycle_type target)
{
  if ( READ_PWM_DEBUG )
  {
    INT16S conv;
    conv = (INT16S) target.motorA;
    PRINTF("pwm A: %d\t",conv);
    conv = (INT16S) target.motorB;
    PRINTF("pwm B: %d\n",conv);
  }
}

void ctrl_task(void *pvParameters)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  pwm_duty_cycle_type next_pwm;

  motor_pos current_pos;
  motor_pos target_pos;
  coordinate_type target_pos_kart;

  portTickType last_wake_time;
  last_wake_time = xTaskGetTickCount();

  INT8U open_loop_scale= 0;
  INT8U reset = 1;
  message_user_interface_type ui_message;
  PRINTF("control task started\n");
  INT8U write_to_log_en = 0;
  INT8U write_to_log_count = 0;
  while(1)
  {
    led_ryg(1,0,0); //to test timing
    ui_message = control_get_state();
    current_pos = spi_read_encoders();
    switch(ui_message.state)
    {

    case SINGLE_ANGLE_POSITION:
      target_pos = *( (motor_pos *)(ui_message.msg) );
      next_pwm.motorA = p_controller_safe(target_pos.positionA, current_pos.positionA);
      next_pwm.motorB = p_controller_safe(target_pos.positionB, current_pos.positionB);
      next_pwm = account_for_deadband(next_pwm);
      write_to_log_en = 10;
      break;

    case SINGLE_CARTESIAN_POSITION:
      target_pos_kart = *( (coordinate_type *)(ui_message.msg) );
      target_pos = coordinate_transform(target_pos_kart);
      next_pwm.motorA = p_controller_safe(target_pos.positionA, current_pos.positionA);
      next_pwm.motorB = p_controller_safe(target_pos.positionB, current_pos.positionB);
      next_pwm = account_for_deadband(next_pwm);
      write_to_log_en = 100;
      break;

    case SINGLE_PWM_MODE:
      next_pwm = *( (pwm_duty_cycle_type *)(ui_message.msg) );
      write_to_log_en = 100;
      break;

    case STOP_MOTORS_NOW:
      next_pwm.motorA = 0;
      next_pwm.motorB = 0;
      reset = 1;
      write_to_log_en = 0;
      write_to_log_count = 0;
      break;

    case SKEET_SHOOT_DEMO:
      target_pos_kart = read_pos_kart(reset);
      target_pos = coordinate_transform(target_pos_kart);
      next_pwm.motorB = pidf_controller_pan(target_pos, current_pos, reset);
//      next_pwm.motorB = pid_controller_pan(target_pos, current_pos, reset);
      next_pwm.motorA = pid_controller_tilt(target_pos, current_pos, reset);
      next_pwm = account_for_deadband(next_pwm);
      reset = 0;
      write_to_log_en = 1;
      write_to_log_count = 0;
      break;

    case OPEN_LOOP_TEST:
      open_loop_scale++;
      if(open_loop_scale == CTRL_PER_PWM)
      {
        open_loop_scale= 0;
        next_pwm = read_pwm_function( reset );
        reset = 0;
        write_to_log_en = 1;
        write_to_log_count = 0;
      }
      break;

    default:
      break;
    }
    write_to_log_count++;
    if(write_to_log_en && (write_to_log_count == write_to_log_en) )
    {
      write_to_log_count = 0;
      log_entry_register(next_pwm, current_pos, target_pos);
    }
    spi_send_pwm(next_pwm);

    led_ryg(0,0,0); //to test timing
    vTaskDelayUntil(&last_wake_time, CTRL_TASK_CYCLE);
  }
}

motor_pos coordinate_transform(coordinate_type coord)
// This function transforms from kartesian to spherical coordinates.
{
  motor_pos return_value;
  return_value.positionA = ( atan(coord.z/(sqrt(pow(coord.x,2) + pow(coord.y,2)))) * 180/PI )*3; //phi
  return_value.positionB = ( atan(coord.y/coord.x) * 180/PI )* 3; //theta

  if(return_value.positionA < 0)
  {
    return_value.positionA += 1079;
  }
  if(return_value.positionB < 0)
  {
    return_value.positionB += 1079;
  }
  return return_value;
}

motor_pos get_target_position()
{
  static motor_pos target;
  coordinate_type coord;

  target.positionA = (90- ( atan((sqrt(pow(coord.x,2) + pow(coord.y,2)))/coord.z) * 180/PI ))*3; //phi
  target.positionB = ( atan(coord.y/coord.x) * 180/PI )*3; //theta

  if(target.positionA < 0)
  {
    target.positionA += 1079;
  }
  if(target.positionB < 0)
  {
    target.positionB += 1079;
  }
  target_pos_debug(target);

  return target;
}

message_user_interface_type control_get_state( void )
{
  static message_user_interface_type interface_message = {.state=STOP_MOTORS_NOW, .msg=(INT8U) 0 };
  xQueueReceive(interface_to_control_queue, &interface_message, 0);
  return interface_message;
}

void control_set_state( INT8U state, void *msg )
{
  message_user_interface_type message;
  message.state = state;
  message.msg = msg;
  xQueueSend(interface_to_control_queue, &message, 0);
}

