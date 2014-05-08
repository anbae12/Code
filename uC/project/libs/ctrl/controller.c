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

/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "SPI/spi.h"  //Needed for position struct
#include "controller.h"


#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"


/*****************************    Defines    *******************************/

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
  // Jeg vil gerne lade disse afhænge af SAMPLE_BUFFER_SIZE, 
  // men så ved jeg ikke hvordan de skal initialiseres til 0
  static FP32 input[5] = {0, 0, 0, 0, 0};
  static FP32 output[5] = {0, 0, 0, 0, 0};
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
