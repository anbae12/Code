/*****************************************************************************
 * University of Southern Denmark
 * Semesterproject Spring 2014
 *
 * MODULENAME.: controller.h
 *
 * PROJECT....: Pan and tilt project
 *
 * DESCRIPTION: Implements the controllers for the Pan and tilt system
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140508  MSC   Created
 *
 ****************************************************************************/
#define TICKS_PER_REVOLOTION 1080
/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "SPI/spi.h"  //Needed for position struct
#include "controller.h"

#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "configs/project_settings.h"


/*****************************    Defines    *******************************/
#define HALF_ONE 1
#define HALF_TWO 2
#define SAME_HALF_ONE 3
#define SAME_HALF_TWO 4
#define DIFFERENT_CUR_HALF_ONE 5
#define DIFFERENT_CUR_HALF_TWO 6

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
INT16S pid_controller_tilt(motor_pos target_pos, motor_pos current_pos)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  FP32 derivative;
  FP32 error;
  FP32 return_value;
  
  FP32 dt = 0.001666660;  // Insert sample period here xD
  // Coefficients: 
  FP32 Kp = CONTROL_TILT_P;
  FP32 Ki = CONTROL_TILT_I;          //1493.7;
  FP32 Kd = CONTROL_TILT_D;
  
  // Pan controller: 
  error = target_pos.positionA - current_pos.positionA;

  if ( ( error < -(TICKS_PER_REVOLOTION/2) ) )
  {
    error += TICKS_PER_REVOLOTION;
  }
  else if(error > (TICKS_PER_REVOLOTION/2) )
  {
    error -= TICKS_PER_REVOLOTION;
  }
  error *= -1;
  integral += error * dt;
  derivative = (error - previous_error)/dt;
  
  return_value = Kp*error + Ki*integral + Kd*derivative; 

  previous_error = error;
  
//  INT32S conv;
//  conv = (INT32S) error;
//  PRINTF("error: %d\n", conv);

  return (INT16S) return_value;
  
}

INT16S pid_controller_pan(motor_pos target_pos, motor_pos current_pos)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  FP32 derivative;
  FP32 error;
  FP32 return_value;

  FP32 dt = 0.001666660;  // Insert sample period here
  // Coefficients:
  FP32 Kp = CONTROL_PAN_P;
  FP32 Ki = CONTROL_PAN_I;
  FP32 Kd = CONTROL_PAN_D;
  INT8U state;
  INT8U position_half;
  INT8U current_half;


  error = target_pos.positionB - current_pos.positionB;

    if ( ( error < -(TICKS_PER_REVOLOTION/2) ) )
    {
      error += TICKS_PER_REVOLOTION;
    }
    else if(error > (TICKS_PER_REVOLOTION/2) )
    {
      error -= TICKS_PER_REVOLOTION;
    }
    error *= -1;

  integral += error * dt;
  derivative = (error - previous_error)/dt;

  return_value = Kp*error + Ki*integral + Kd*derivative;

  previous_error = error;

  return (INT16S) return_value;
}

INT16S p_controller_safe(FP32 target_pos, FP32 current_pos)
{
  // Variables
  FP32 error;
  FP32 return_value;

  FP32 Kp = 10;

  error = target_pos - current_pos;

    if ( ( error < -(TICKS_PER_REVOLOTION/2) ) )
    {
      error += TICKS_PER_REVOLOTION;
    }
    else if(error > (TICKS_PER_REVOLOTION/2) )
    {
      error -= TICKS_PER_REVOLOTION;
    }
    error *= -1;

  return_value = Kp*error;

  return (INT16S) return_value;
}

pwm_duty_cycle_type account_for_deadband(pwm_duty_cycle_type pwm)
{
  pwm_duty_cycle_type output;
  if( (pwm.motorA < DEAD_BAND_TILT) && (pwm.motorA > 10))
  {
    output.motorA = DEAD_BAND_TILT;
  }
  if((pwm.motorB < DEAD_BAND_PAN) && (pwm.motorB > 10))
  {
    output.motorB = DEAD_BAND_PAN;
  }
  else output = pwm;
  return output;
}
