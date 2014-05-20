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
#define DEAD_BAND_TILT 245    //tested dead band for tilt (12 % * 2048 = 245)
#define DEAD_BAND_PAN  204    //tested dead band pan ( 10 % * 2048 = 204)

#define DEAD_BAND_TILT_MIN  20  //1 %
#define DEAD_BAND_PAN_MIN   20  //1 %

//Parameters for PID Controller with no d-filter
//Set 1 (Mathematical model)
//        Kp      0.01815142422074102760000638399228    *2048
//        Ki      0.01628973968528040938462111383923    *2048
//        Kd      0.00010530153153699121780772934303215 *2048
//
//
//Set 2 (Simulink DC Motor model)
//        Kp      0.00791710168885304                   *2048
//        Ki      0.00754696305814429                   *2048
//        Kd      -0.0000212802788271494                *2048  //NEGATIVE!!!

//#define CONTROL_TILT_P 74.4933      //within 2 degrees...
//#define CONTROL_TILT_I 435.0662
//#define CONTROL_TILT_D 0.24896
//
//#define CONTROL_PAN_P  74.8392
//#define CONTROL_PAN_I  435.8195;      //1670.4;
//#define CONTROL_PAN_D   0.24264;

#define CONTROL_TILT_P  37.174
#define CONTROL_TILT_I  33.361
#define CONTROL_TILT_D  0.215658

#define CONTROL_PAN_P  37.174
#define CONTROL_PAN_I  33.361
#define CONTROL_PAN_D  0.215658


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

INT16S pid_controller_tilt(motor_pos target_pos, motor_pos current_pos);
INT16S pid_controller_pan(motor_pos target_pos, motor_pos current_pos);
INT16S p_controller_safe(FP32 target_pos, FP32 current_pos);

pwm_duty_cycle_type account_for_deadband(pwm_duty_cycle_type pwm);
