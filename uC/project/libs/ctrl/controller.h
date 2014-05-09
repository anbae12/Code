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
#include "spi.h"  //Needed for position struct



#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"


/*****************************    Defines    *******************************/
#define   SAMPLE_BUFFER_SIZE          5

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

INT16S pan_controller(motor_pos target_pos, motor_pos current_pos);


extern pwm_duty_cycle_type test_controller(motor_pos target_pos, motor_pos current_pos);
