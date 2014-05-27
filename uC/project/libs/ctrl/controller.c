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
INT16S pid_controller_tilt(motor_pos target_pos, motor_pos current_pos, INT8U reset)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  if(reset)
  {
    integral = 0;
  }
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
  if(integral > INTEGRAL_SATURATION_TILT)
  {
    integral = INTEGRAL_SATURATION_TILT;
  }
  else if(integral < -INTEGRAL_SATURATION_TILT)
  {
    integral = -INTEGRAL_SATURATION_TILT;
  }
//  INT32S conv;
//  conv = (INT32S) integral;
//  PRINTF("INTEGRAL: %d\n",conv);
  derivative = (error - previous_error)/dt;
  
  return_value = Kp*error + Ki*integral + Kd*derivative; 

  previous_error = error;
  
//  INT32S conv;
//  conv = (INT32S) error;
//  PRINTF("error: %d\n", conv);
  //to avoid overflow
  if(return_value > 9999)
  {
    return_value = 9999;
  }
  else if(return_value < - 9999)
  {
    return_value = -9999;
  }
//    INT16S conv;
//    conv = (INT16S) return_value;
//    PRINTF("error: %d\n", conv);

  return (INT16S) return_value;
  
}

INT16S pid_controller_pan(motor_pos target_pos, motor_pos current_pos, INT8U reset)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  if(reset)
  {
    integral = 0;
  }
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
  if(integral >= INTEGRAL_SATURATION_PAN)
  {
    integral = INTEGRAL_SATURATION_PAN;
  }
  else if(integral <= -INTEGRAL_SATURATION_PAN)
  {
    integral = -INTEGRAL_SATURATION_PAN;
  }


  derivative = (error - previous_error)/dt;

  return_value = Kp*error + Ki*integral + Kd*derivative;

  previous_error = error;

  //to avoid overflow
  if(return_value > 32767)
  {
    return_value = 32767;
  }
  else if(return_value < - 32767)
  {
    return_value = -32767;
  }

  return (INT16S) return_value;
}

INT16S pidf_controller_pan(motor_pos target_pos, motor_pos current_pos, INT8U reset)
{
  // Variables
  static FP32 previous_error = 0;
  static FP32 integral = 0;
  if(reset)
  {
    integral = 0;
  }
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
  if(integral > INTEGRAL_SATURATION_PAN)
  {
    integral = INTEGRAL_SATURATION_PAN;
  }
  else if(integral < -INTEGRAL_SATURATION_PAN)
  {
    integral = -INTEGRAL_SATURATION_PAN;
  }

  derivative = (error - previous_error)/dt;

  // Filter the derivative
  derivative = pan_derivative_filter(derivative);

  return_value = Kp*error + Ki*integral + Kd*derivative;

  previous_error = error;

  return (INT16S) return_value;
}

FP32 pan_derivative_filter(FP32 derivative)
// Implements a 4th order (5 taps), Kaiser window FIR.
// Beta = 3.4, Fs = 600, Fc = 120.
// Filter was designed using Matlab's fdatool
{
	static FP32 input[PAN_DFILTER_TAPS]; //input samples
	const FP32 fir_coef[PAN_DFILTER_TAPS] = {
			0.0163360130965946,
			0.246703930668704,
			0.473920112469402,
			0.246703930668704,
			0.0163360130965946};
	INT8U n;
	FP32 output = 0;
	//shift the old samples
	for(n=PAN_DFILTER_TAPS-1; n>0; n--)
	 input[n] = input[n-1];

	input[0] = derivative;

	//Calculate the new output
	for(n=0; n<PAN_DFILTER_TAPS; n++)
	{
		output += input[n] * fir_coef[n];
	}
	return output;
}


INT16S p_controller_safe(FP32 target_pos, FP32 current_pos)
{
  // Variables
  FP32 error;
  FP32 return_value;

  FP32 Kp = 21;

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

  if(return_value > MAX_PWM_SAFE)
    return_value = MAX_PWM_SAFE; //This controller should be SAFE!
  if(return_value < -MAX_PWM_SAFE)
    return_value = -MAX_PWM_SAFE;

  return (INT16S) return_value;
}

pwm_duty_cycle_type account_for_deadband(pwm_duty_cycle_type pwm)
{
  pwm_duty_cycle_type output = pwm;
  if( (pwm.motorA < DEAD_BAND_TILT) && (pwm.motorA > DEAD_BAND_TILT_MIN)) //<245 && >20
  {
    output.motorA = DEAD_BAND_TILT;
  }
  else if( (pwm.motorA > -DEAD_BAND_TILT) && (pwm.motorA < -DEAD_BAND_TILT_MIN))  //>-245 && <-20
  {
    output.motorA = -DEAD_BAND_TILT;
  }
  if((pwm.motorB < DEAD_BAND_PAN) && (pwm.motorB > DEAD_BAND_PAN_MIN)) //<204 && >20
  {
    output.motorB = DEAD_BAND_PAN;
  }
  else if( (pwm.motorB > -DEAD_BAND_PAN) && (pwm.motorB < -DEAD_BAND_PAN_MIN)) //>-204 && <-20
  {
    output.motorB = -DEAD_BAND_PAN;
  }
  return output;
}
