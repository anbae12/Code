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
#define DEAD_BAND_TILT 141    //tested dead band for tilt
#define DEAD_BAND_PAN  221    //tested dead band pan

#define CONTROL_TILT_P 74.4933      //within 2 degrees...
#define CONTROL_TILT_I 435.0662
#define CONTROL_TILT_D 0.24896

#define CONTROL_PAN_P  74.8392
#define CONTROL_PAN_I  435.8195;      //1670.4;
#define CONTROL_PAN_D   0.24264;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

INT16S pid_controller_tilt(motor_pos target_pos, motor_pos current_pos);
INT16S pid_controller_pan(motor_pos target_pos, motor_pos current_pos);
INT16S p_controller_safe(FP32 target_pos, FP32 current_pos);

pwm_duty_cycle_type account_for_deadband(pwm_duty_cycle_type pwm);
