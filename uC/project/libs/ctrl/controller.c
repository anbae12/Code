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
 *****************************************************************************
 * TODO:
 *   Initialize function
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

INT16S pan_controller(motor_pos target_pos, motor_pos current_pos) 
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : Implements the pan controller.
 ******************************************************************************/
{
  // Initializes to 0:
  static FP32 input[5];
  static FP32 output[5];
  
  FP32 a_coef[5] = {0, 0, 0, 0, 0}; // a[0] skal være = 0.
  FP32 b_coef[5] = {1, 0, 0.03, 0.01, 0};
  
  INT8U i;
  
  // Shift buffers:
  for (i = 0; i < 4; i++)
  {
    output[i-1] = output[i];
    input[i-1] = input[i];
  }
  
   
  input[0] = target_pos.positionA - current_pos.positionA;
  output[0] = 0;
  
  // Differensligningen
  for (i = 0; i < 5; i++)
  {
    output[0] += input[i]*b_coef[i] - output[i]*a_coef[i];
  }
  // asd
  
  return (int) output[0];
}

INT16S pid_controller_tilt(motor_pos target_pos, motor_pos current_pos)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  FP32 derivative;
  FP32 error;
  FP32 return_value;
  
  FP32 dt = 0.001667;  // Insert sample period here xD
  // Coefficients: 
  FP32 Kp = 15;
  FP32 Ki = 0;
  FP32 Kd = 0; 
  
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



pwm_duty_cycle_type test_controller(motor_pos target_pos, motor_pos current_pos)
/*****************************************************************************
 *   Input    : positions
 *   Output   : pwm
 *   Function : Test function to see if basic stuff works.
 ******************************************************************************/
{
  pwm_duty_cycle_type next_pwm;


  if(target_pos.positionA >= current_pos.positionA )
  {
    next_pwm.motorA = 20 * PWM_PERCENT;
  }
  else if(target_pos.positionA < current_pos.positionA)
  {
    next_pwm.motorA = 30 * PWM_PERCENT;
  }

/*
  if(target_pos.positionB <= current_pos.positionB + TEST_OFFSET_C)
  {
    next_pwm.motorB = 60 * PWM_PERCENT;
  }
  else if(target_pos.positionB <= current_pos.positionB + TEST_OFFSET_B)
  {
    next_pwm.motorB = 50 * PWM_PERCENT;
  }
  else if(target_pos.positionB <= current_pos.positionB + TEST_OFFSET_A)
  {
    next_pwm.motorB = 30 * PWM_PERCENT;
  }
  else
  */
  next_pwm.motorB = 0 * PWM_PERCENT;

  return next_pwm;
}



INT16S pid_controller_pan(motor_pos target_pos, motor_pos current_pos)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  FP32 derivative;
  FP32 error;
  FP32 return_value;

  FP32 dt = 0.001667;  // Insert sample period here xD
  // Coefficients:
  FP32 Kp = 10;
  FP32 Ki = 0;
  FP32 Kd = 0;
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

  if (target_pos.positionB < 900 && target_pos.positionB > 200) //only for safety - can be removed if you wear gloves and goggles.
  {
          return_value = 0;
  }

  return (INT16S) return_value;
}



